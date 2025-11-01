#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Renderer/Mesh/Mesh.h"

class Cube : public Mesh
{
public:
    float size;
    glm::vec3 normal;
    glm::vec3 color;

    Cube(float size = 1.0f, glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));

private:
    static std::vector<Vertex> generateVertices(float size, glm::vec3 normal, glm::vec3 color);
    static std::vector<GLuint> generateIndices();
};

#endif
