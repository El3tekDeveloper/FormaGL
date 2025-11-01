#include "Geometry/Plane.h"

Plane::Plane(float size, glm::vec3 normal, glm::vec3 color)
    : Mesh(generateVertices(size, normal, color), generateIndices(), std::vector<Texture>{})
{
    Plane::normal = normal;
    Plane::color = color;
}

std::vector<Vertex> Plane::generateVertices(float size, glm::vec3 normal, glm::vec3 color)
{
    float s = size * 0.5f;
    
    return {
        Vertex{ glm::vec3(-s, 0.0f,  s), normal, color, glm::vec2(0.0f, 1.0f) },
        Vertex{ glm::vec3( s, 0.0f,  s), normal, color, glm::vec2(1.0f, 1.0f) },
        Vertex{ glm::vec3( s, 0.0f, -s), normal, color, glm::vec2(1.0f, 0.0f) },
        Vertex{ glm::vec3(-s, 0.0f, -s), normal, color, glm::vec2(0.0f, 0.0f) }
    };
}

std::vector<GLuint> Plane::generateIndices()
{
    return { 0, 1, 2, 0, 2, 3 };
}
