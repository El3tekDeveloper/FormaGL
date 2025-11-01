#ifndef PLANE_CLASS_H
#define PLANE_CLASS_H

#include "Renderer/Mesh/Mesh.h"

class Plane : public Mesh
{
public:
    glm::vec3 normal;
    glm::vec3 color;
    
    Plane(float size = 1.0f, glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
private:
    static std::vector<Vertex> generateVertices(float size, glm::vec3 normal, glm::vec3 color);
    static std::vector<GLuint> generateIndices();
};

#endif
