#include "Geometry/Cube.h"

Cube::Cube(float size, glm::vec3 normal, glm::vec3 color)
    : Mesh(generateVertices(size, normal, color), generateIndices(), std::vector<Texture>{})
{
    Cube::size = size;
    Cube::normal = normal;
    Cube::color = color;
}

std::vector<Vertex> Cube::generateVertices(float size, glm::vec3 normal, glm::vec3 color)
{
    float s = size * 0.5f;
    return {
        Vertex{glm::vec3(-s, -s,  s), normal, color, glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-s, -s, -s), normal, color, glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( s, -s, -s), normal, color, glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( s, -s,  s), normal, color, glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-s,  s,  s), normal, color, glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-s,  s, -s), normal, color, glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( s,  s, -s), normal, color, glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( s,  s,  s), normal, color, glm::vec2(0.0f, 1.0f)}
    };
}

std::vector<GLuint> Cube::generateIndices()
{
    return {
        0, 1, 2, 0, 2, 3,
        0, 4, 7, 0, 7, 3,
        3, 7, 6, 3, 6, 2,
        2, 6, 5, 2, 5, 1,
        1, 5, 4, 1, 4, 0,
        4, 5, 6, 4, 6, 7
    };
}
