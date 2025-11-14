#pragma once
#include <iostream>
#include <string>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tinyexr/tinyexr.h>
#include <stb/stb_image.h>

#include "Core/ShaderClass.h"
#include "Core/Camera.h"
#include "Geometry/Cube.h"

class Skybox
{
public:
    GLuint textureID;
    Cube* cube;

    Skybox(const char* hdrPath);
    ~Skybox();
    void Draw(Shader& shader, Camera& camera);

private:
    GLuint loadHDRTexture(const char* path);
    GLuint loadEXRTexture(const char* path);
    GLuint loadSTBIHDRTexture(const char* path);
};