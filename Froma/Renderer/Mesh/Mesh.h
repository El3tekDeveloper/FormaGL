#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <string>
#include <vector>

#include "Renderer/Buffers/VAO.h"
#include "Renderer/Buffers/EBO.h"
#include "Core/Camera.h"
#include "Renderer/Mesh/Texture.h"
#include "Core/ShaderClass.h"

class Mesh
{
    public:
        std::vector <Vertex> vertices;
        std::vector <GLuint> indices;
        std::vector <Texture> textures;

        VAO VAO;
        Mesh(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices, const std::vector <Texture>& textures);

        void Draw(Shader& shader, Camera& camera,
            glm::mat4 matrix = glm::mat4(1.0f),
            glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif