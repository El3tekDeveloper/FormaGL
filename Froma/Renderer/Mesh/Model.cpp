#include "Model.h"

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();

	// Start traversal from scene's root nodes
	if (JSON.contains("scenes") && JSON.contains("scene"))
	{
		unsigned int sceneIdx = JSON["scene"];
		json scene = JSON["scenes"][sceneIdx];
		
		if (scene.contains("nodes"))
		{
			for (unsigned int i = 0; i < scene["nodes"].size(); i++)
			{
				traverseNode(scene["nodes"][i]);
			}
		}
	}
	else if (JSON.contains("nodes"))
	{
		// Fallback: traverse from node 0
		traverseNode(0);
	}
}

void Model::Draw(Shader& shader, Camera& camera, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i], translation, rotation, scale);
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	if (!JSON.contains("meshes") || indMesh >= JSON["meshes"].size())
		return;

	json mesh = JSON["meshes"][indMesh];
	
	if (!mesh.contains("primitives"))
		return;

	// Support multiple primitives per mesh
	for (unsigned int primIdx = 0; primIdx < mesh["primitives"].size(); primIdx++)
	{
		json primitive = mesh["primitives"][primIdx];
		
		if (!primitive.contains("attributes"))
			continue;

		json attributes = primitive["attributes"];

		// Check for required POSITION attribute
		if (!attributes.contains("POSITION"))
			continue;

		unsigned int posAccInd = attributes["POSITION"];
		std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
		std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

		// Normals (optional, generate if missing)
		std::vector<glm::vec3> normals;
		if (attributes.contains("NORMAL"))
		{
			unsigned int normalAccInd = attributes["NORMAL"];
			std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
			normals = groupFloatsVec3(normalVec);
		}
		else
		{
			normals.resize(positions.size(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		// Texture coordinates (optional)
		std::vector<glm::vec2> texUVs;
		if (attributes.contains("TEXCOORD_0"))
		{
			unsigned int texAccInd = attributes["TEXCOORD_0"];
			std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
			texUVs = groupFloatsVec2(texVec);
		}
		else
		{
			texUVs.resize(positions.size(), glm::vec2(0.0f, 0.0f));
		}

		// Vertex colors (optional)
		std::vector<glm::vec3> colors;
		if (attributes.contains("COLOR_0"))
		{
			unsigned int colorAccInd = attributes["COLOR_0"];
			json colorAccessor = JSON["accessors"][colorAccInd];
			std::string colorType = colorAccessor["type"];
			
			std::vector<float> colorVec = getFloats(colorAccessor);
			
			if (colorType == "VEC3")
			{
				colors = groupFloatsVec3(colorVec);
			}
			else if (colorType == "VEC4")
			{
				std::vector<glm::vec4> colors4 = groupFloatsVec4(colorVec);
				for (const auto& c : colors4)
					colors.push_back(glm::vec3(c.x, c.y, c.z));
			}
		}
		else
		{
			colors.resize(positions.size(), glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// Indices (optional, generate if missing)
		std::vector<GLuint> indices;
		if (primitive.contains("indices"))
		{
			unsigned int indAccInd = primitive["indices"];
			indices = getIndices(JSON["accessors"][indAccInd]);
		}
		else
		{
			// Non-indexed geometry
			for (unsigned int i = 0; i < positions.size(); i++)
				indices.push_back(i);
		}

		// Get textures from material
		std::vector<Texture> textures;
		if (primitive.contains("material"))
		{
			unsigned int matIdx = primitive["material"];
			textures = getTextures(matIdx);
		}

		std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs, colors);
		meshes.push_back(Mesh(vertices, indices, textures));
	}
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	if (!JSON.contains("nodes") || nextNode >= JSON["nodes"].size())
		return;

	json node = JSON["nodes"][nextNode];

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.contains("translation"))
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.contains("rotation"))
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.contains("scale"))
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}

	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.contains("matrix"))
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.contains("mesh"))
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	if (node.contains("children"))
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> Model::getData()
{
	if (!JSON.contains("buffers") || JSON["buffers"].size() == 0)
		return std::vector<unsigned char>();

	std::string uri = JSON["buffers"][0]["uri"];
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Check if URI is base64 encoded data
	if (uri.find("data:") == 0)
	{
		size_t commaPos = uri.find(',');
		if (commaPos != std::string::npos)
		{
			std::string base64Data = uri.substr(commaPos + 1);
			return decodeBase64(base64Data);
		}
	}

	// Load from external file
	std::string bytesText = get_file_contents((fileDirectory + uri).c_str());
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<unsigned char> Model::decodeBase64(const std::string& encoded)
{
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	std::vector<unsigned char> decoded;
	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

	int val = 0, valb = -8;
	for (unsigned char c : encoded)
	{
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0)
		{
			decoded.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return decoded;
}

std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;

	floatVec.reserve(count * numPerVert);

	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4)
	{
		float value;
		std::memcpy(&value, &data[i], sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	unsigned int beginningOfData = byteOffset + accByteOffset;

	indices.reserve(count);

	if (componentType == 5125) // UNSIGNED_INT
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i += 4)
		{
			unsigned int value;
			std::memcpy(&value, &data[i], sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123) // UNSIGNED_SHORT
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i += 2)
		{
			unsigned short value;
			std::memcpy(&value, &data[i], sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122) // SHORT
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i += 2)
		{
			short value;
			std::memcpy(&value, &data[i], sizeof(short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5121) // UNSIGNED_BYTE
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + count; i++)
		{
			indices.push_back((GLuint)data[i]);
		}
	}

	return indices;
}

std::vector<Texture> Model::getTextures(unsigned int matIdx)
{
	std::vector<Texture> textures;

	if (!JSON.contains("materials") || matIdx >= JSON["materials"].size())
		return textures;

	json material = JSON["materials"][matIdx];
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// PBR Metallic Roughness workflow
	if (material.contains("pbrMetallicRoughness"))
	{
		json pbr = material["pbrMetallicRoughness"];

		// Base Color Texture (diffuse)
		if (pbr.contains("baseColorTexture"))
		{
			unsigned int texIdx = pbr["baseColorTexture"]["index"];
			loadTextureFromIndex(texIdx, "diffuse", fileDirectory, textures);
		}

		// Metallic Roughness Texture (specular)
		if (pbr.contains("metallicRoughnessTexture"))
		{
			unsigned int texIdx = pbr["metallicRoughnessTexture"]["index"];
			loadTextureFromIndex(texIdx, "specular", fileDirectory, textures);
		}
	}

	// Normal Map
	if (material.contains("normalTexture"))
	{
		unsigned int texIdx = material["normalTexture"]["index"];
		loadTextureFromIndex(texIdx, "normal", fileDirectory, textures);
	}

	// Occlusion Map
	if (material.contains("occlusionTexture"))
	{
		unsigned int texIdx = material["occlusionTexture"]["index"];
		loadTextureFromIndex(texIdx, "occlusion", fileDirectory, textures);
	}

	// Emissive Map
	if (material.contains("emissiveTexture"))
	{
		unsigned int texIdx = material["emissiveTexture"]["index"];
		loadTextureFromIndex(texIdx, "emissive", fileDirectory, textures);
	}

	return textures;
}

void Model::loadTextureFromIndex(unsigned int texIdx, const char* type, const std::string& directory, std::vector<Texture>& textures)
{
	if (!JSON.contains("textures") || texIdx >= JSON["textures"].size())
		return;

	json texture = JSON["textures"][texIdx];
	
	if (!texture.contains("source"))
		return;

	unsigned int imgIdx = texture["source"];
	
	if (!JSON.contains("images") || imgIdx >= JSON["images"].size())
		return;

	json image = JSON["images"][imgIdx];
	
	// Handle URI-based images
	if (image.contains("uri"))
	{
		std::string texPath = image["uri"];
		
		// Check if already loaded
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				// Find the corresponding loaded texture with same type
				for (unsigned int k = 0; k < loadedTex.size(); k++)
				{
					if (loadedTexName[k] == texPath)
					{
						textures.push_back(loadedTex[k]);
						return;
					}
				}
			}
		}

		// Load new texture
		std::string fullPath = directory + texPath;
		Texture tex = Texture(fullPath.c_str(), type, loadedTex.size());
		textures.push_back(tex);
		loadedTex.push_back(tex);
		loadedTexName.push_back(texPath);
	}
	// Handle embedded images (bufferView)
	else if (image.contains("bufferView"))
	{
		// For now, skip embedded images
		// You can implement this by extracting data from bufferView
		// and creating texture from memory
	}
}

std::vector<Vertex> Model::assembleVertices(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs,
	std::vector<glm::vec3> colors)
{
	std::vector<Vertex> vertices;
	vertices.reserve(positions.size());

	for (size_t i = 0; i < positions.size(); i++)
	{
		vertices.push_back(Vertex{
			positions[i],
			normals[i],
			colors[i],
			texUVs[i]
		});
	}
	return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	vectors.reserve(floatVec.size() / 2);

	for (size_t i = 0; i < floatVec.size(); i += 2)
	{
		vectors.emplace_back(floatVec[i], floatVec[i + 1]);
	}
	return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	vectors.reserve(floatVec.size() / 3);

	for (size_t i = 0; i < floatVec.size(); i += 3)
	{
		vectors.emplace_back(floatVec[i], floatVec[i + 1], floatVec[i + 2]);
	}
	return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	vectors.reserve(floatVec.size() / 4);

	for (size_t i = 0; i < floatVec.size(); i += 4)
	{
		vectors.emplace_back(floatVec[i], floatVec[i + 1], floatVec[i + 2], floatVec[i + 3]);
	}
	return vectors;
}