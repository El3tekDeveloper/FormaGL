#pragma once
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tinyexr/tinyexr.h>

#include "Core/ShaderClass.h"
#include "Core/Camera.h"
#include "Geometry/Cube.h"

class Skybox
{
public:
    GLuint textureID;
    Cube* cube;

    Skybox(const char* exrPath);
    ~Skybox();
    void Draw(Shader& shader, Camera& camera);

private:
    GLuint loadEXRTexture(const char* path);
};