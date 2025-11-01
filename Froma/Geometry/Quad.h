#ifndef QUAD_CLASS_H
#define QUAD_CLASS_H

#include "Renderer/Mesh/Mesh.h"

class Quad : public Mesh
{
public:
    Quad();
private:
    static std::vector<Vertex> generateVertices();
    static std::vector<GLuint> generateIndices();
};

#endif
