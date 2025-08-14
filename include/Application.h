#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SimInfo/fpsTracker.h"

#include "Rendering/Wrappers/Shader.h"
#include "Rendering/cubeData.h"
#include "Rendering/Wrappers/VAO.h"
#include "Rendering/Wrappers/VBO.h"
#include "Rendering/Wrappers/EBO.h"
#include "Rendering/Camera.h"
#include "Rendering/Renderer.h"
#include "Helpers.h"


class Application
{
public:

    Application(int width, int height, const char* title);
    ~Application();

    void run();

private:

    void setupScene();
    void processInputs();
    void update();
    void render();

    void toggleFullscreen();

    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<Renderer> renderer;
    FpsTracker fpsTracker;

    std::vector<std::shared_ptr<Entity>> dynamicEntities;
    std::vector<std::shared_ptr<Entity>> staticEntities;

    float deltaTime;
    float lastFrame;

    int windowedPosX, windowedPosY;
    int windowedWidth, windowedHeight;
    bool isFullscreen = false;

    
};