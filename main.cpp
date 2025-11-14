
#include <iostream>
#include <cmath>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Froma/Core/Camera.h>
#include <Froma/Core/ShaderClass.h>
#include <Froma/Renderer/Buffers/VAO.h>
#include <Froma/Renderer/Buffers/VBO.h>
#include <Froma/Renderer/Buffers/EBO.h>
#include <Froma/Renderer/Mesh/Texture.h>
#include <Froma/Renderer/Mesh/Mesh.h>
#include <Froma/Renderer/Mesh/Model.h>
#include <Froma/Renderer/Skybox/Skybox.h>
#include <Froma/Geometry/GeometryCollection.h>

GLFWwindow* window = nullptr;
const char* title = "Graphic engine";

int WIDTH = 1920;
int HEIGHT = 1080;

std::string basePath = std::filesystem::current_path().string();
const std::string texturesPath = basePath + "/assets/textures/";
const std::string shadersPath  = basePath + "/shaders/";
const std::string modelsPath   = basePath + "/assets/models/";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);

    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
    {
        cam->width = width;
        cam->height = height;
    }
} 

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    gladLoadGL();
    glViewport(0, 0, WIDTH, HEIGHT);

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    glfwSetWindowUserPointer(window, &camera);

    Texture textures[]
	{
        Texture((texturesPath + "planks.png").c_str(), "diffuse", 0),
		Texture((texturesPath + "planksSpec.png").c_str(), "specular", 1)
	};
    struct lightConfig
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        float lightColor[4] = {1, 1, 1, 1};
        int lightType = 0;
    };
    
    lightConfig lightSettings;
    lightSettings.lightType = 1;

    Shader floorShader((shadersPath + "default.vert").c_str(), (shadersPath + "default.frag").c_str());
    Shader skyboxShader((shadersPath + "skybox.vert").c_str(), (shadersPath + "skybox.frag").c_str());
    Shader characterShader((shadersPath + "default.vert").c_str(), (shadersPath + "default.frag").c_str());

    GeometryCollection::Plane floor(15);
    glm::mat4 floorModel = glm::mat4(1.0f);
    glm::vec3 floorPosition = glm::vec3(0, 2, 0);
    floor.textures = std::vector <Texture>(textures, textures + sizeof(textures) / sizeof(Texture));
    
    floorShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(floorShader.ID, "model"), 0, GL_FALSE, glm::value_ptr(floorModel));
    glUniform4f(glGetUniformLocation(floorShader.ID, "lightColor"), lightSettings.lightColor[0], lightSettings.lightColor[1], lightSettings.lightColor[2], lightSettings.lightColor[3]);
    glUniform3f(glGetUniformLocation(floorShader.ID, "lightPos"), lightSettings.position.x, lightSettings.position.y, lightSettings.position.z);
    glUniform1i(glGetUniformLocation(floorShader.ID, "lightType"), lightSettings.lightType);
    
    Model character("C:/Users/youel/Desktop/Projects/C++/FormaGL/assets/models/3D-Character/scene.gltf");
    glm::mat4 characterModel = glm::mat4(1.0f);
    glm::vec3 characterPosition = glm::vec3(0.0f, 2949.0f, 5.0f);
    glm::vec3 characterScale = glm::vec3(2.0f, 2.0f, 2.0f);

    characterShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(characterShader.ID, "model"), 0, GL_FALSE, glm::value_ptr(characterModel));
    glUniform4f(glGetUniformLocation(characterShader.ID, "lightColor"), lightSettings.lightColor[0], lightSettings.lightColor[1], lightSettings.lightColor[2], lightSettings.lightColor[3]);
    glUniform3f(glGetUniformLocation(characterShader.ID, "lightPos"), -lightSettings.position.x, -lightSettings.position.y, -lightSettings.position.z);
    glUniform1i(glGetUniformLocation(characterShader.ID, "lightType"), lightSettings.lightType);
    
    Skybox skybox((texturesPath + "citrus_orchard_puresky_4k.exr").c_str());
    skyboxShader.Activate();

    glClearColor(0.192, 0.301, 0.474, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    bool showUI = false;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.192, 0.301, 0.474, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        camera.Inputs(window);
        camera.UpdateMatrix(45.0f, 0.1, 100.0f);

        static bool ctrlPressed = false;
        ctrlPressed = (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) ? (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && !ctrlPressed ? (showUI = !showUI, true) : true) : false;
        
        if (showUI)
        {    
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(350, (float)HEIGHT));
            ImGui::Begin("Scene Hierarchy", nullptr, window_flags);
            if (ImGui::CollapsingHeader("Light"))
            {  
                ImGui::DragFloat3("Light Position", &lightSettings.position.x);
                const char* lightTypes[] = { "Point", "Directional", "Spot" };
                ImGui::Combo("Light Type", &lightSettings.lightType, lightTypes, IM_ARRAYSIZE(lightTypes));
            }
            if (ImGui::CollapsingHeader("Character"))
            {  
                ImGui::DragFloat3("Position", &characterPosition.x);
                ImGui::DragFloat3("Scale", &characterScale.x);
            }
            ImGui::End();
        }
                
        floorShader.Activate();
        glUniform3f(glGetUniformLocation(floorShader.ID, "lightPos"), lightSettings.position.x, lightSettings.position.y, lightSettings.position.z);
        glUniform1i(glGetUniformLocation(floorShader.ID, "lightType"), lightSettings.lightType);

        characterShader.Activate();
        glUniform3f(glGetUniformLocation(characterShader.ID, "lightPos"), -lightSettings.position.x, -lightSettings.position.y, -lightSettings.position.z);
        glUniform1i(glGetUniformLocation(characterShader.ID, "lightType"), lightSettings.lightType);

        floor.Draw(floorShader, camera, floorModel, floorPosition);
        character.Draw(characterShader, camera, characterPosition, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), characterScale);
        skybox.Draw(skyboxShader, camera);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
