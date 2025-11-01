#include "Geometry/Quad.h"

Quad::Quad()
    : Mesh(generateVertices(), generateIndices(), std::vector<Texture>{})
{
}

std::vector<Vertex> Quad::generateVertices()
{
    return {
        Vertex{glm::vec3(-0.5f, -0.5f, 0.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f, 0.0f)}
    };
}

std::vector<GLuint> Quad::generateIndices()
{
    return { 0, 1, 2, 0, 2, 3 };
}
