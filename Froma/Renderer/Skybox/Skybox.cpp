#include "Skybox.h"

Skybox::Skybox(const char* exrPath)
{
    cube = new Cube(1.0f, glm::vec3(0.0f), glm::vec3(1.0f));
    textureID = loadEXRTexture(exrPath);
}

Skybox::~Skybox()
{
    delete cube;
    glDeleteTextures(1, &textureID);
}

GLuint Skybox::loadEXRTexture(const char* path)
{
    float* out; 
    int width, height;
    const char* err = nullptr;

    int ret = LoadEXR(&out, &width, &height, path, &err);
    if (ret != TINYEXR_SUCCESS)
    {
        std::cerr << "Failed to load EXR: " << (err ? err : "Unknown error") << std::endl;
        if (err) FreeEXRErrorMessage(err);
        return 0;
    }

    std::cout << "Loaded EXR: " << width << "x" << height << std::endl;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, out);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(out);
    return tex;
}

void Skybox::Draw(Shader& shader, Camera& camera)
{
    glDepthFunc(GL_LEQUAL);
    shader.Activate();

    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    glm::mat4 proj = camera.projection;

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader.ID, "equirectangularMap"), 0);

    cube->VAO.Bind();
    glDrawElements(GL_TRIANGLES, cube->indices.size(), GL_UNSIGNED_INT, 0);
    
    glDepthFunc(GL_LESS);
}