#include "Sphere.h"

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 color)
    : Mesh(generateVertices(radius, sectorCount, stackCount, color),
           generateIndices(sectorCount, stackCount),
           std::vector<Texture>{})
{
    Sphere::radius = radius;
    Sphere::sectorCount = sectorCount;
    Sphere::stackCount = stackCount;

    Sphere::color = color;
}

std::vector<Vertex> Sphere::generateVertices(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 color)
{
    std::vector<Vertex> vertices;
    const float PI = 3.14159265359f;

    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = PI / 2 - i * (PI / stackCount);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * (2 * PI / sectorCount);

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(glm::vec3(x, y, z));
            vertex.color = color;
            vertex.textureUV = glm::vec2((float)j / sectorCount, (float)i / stackCount);
            vertices.push_back(vertex);
        }
    }

    return vertices;
}

std::vector<GLuint> Sphere::generateIndices(unsigned int sectorCount, unsigned int stackCount)
{
    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
                indices.insert(indices.end(), { k1, k2, k1 + 1 });

            if (i != (stackCount - 1))
                indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });
        }
    }

    return indices;
}
