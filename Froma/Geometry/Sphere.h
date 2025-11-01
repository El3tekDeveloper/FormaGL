#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include "Renderer/Mesh/Mesh.h"
#include <cmath>

class Sphere : public Mesh
{
public:
    float radius;
    unsigned int sectorCount;
    unsigned int stackCount;

    glm::vec3 color;

    Sphere(float radius = 1.0f, unsigned int sectorCount = 36, unsigned int stackCount = 18, glm::vec3 color = glm::vec3(1.0f));

private:
    static std::vector<Vertex> generateVertices(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 color);
    static std::vector<GLuint> generateIndices(unsigned int sectorCount, unsigned int stackCount);
};

#endif
