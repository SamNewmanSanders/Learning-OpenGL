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


// Forward-declare ImGui structs
struct ImGuiIO;

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    void run();

private:

    // === Internal structs ===
    struct InputState {
        bool firstMouse = true;
        float lastX = 600.0f;
        float lastY = 400.0f;
        bool mouseEnabled = false;
        bool tabPressedLastFrame = false;
    };

    // === Core state ===
    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<Renderer> renderer;
    std::vector<Entity> dynamicEntities;
    std::vector<Entity> staticEntities;

    FpsTracker fpsTracker;

    // Timing
    float deltaTime;
    float lastFrame;

    // Input
    InputState input;

    // Fullscreen
    bool isFullscreen = false;
    int windowedPosX, windowedPosY, windowedWidth, windowedHeight;

    // FUNCTIONS:

    // Setup
    void initWindow(int width, int height, const char* title);
    void initOpenGL();
    void initImGui();
    void setupScene();

    // Main loop
    void processInputs();
    void update();
    void render();

    // Rendering
    void renderUI();
    void renderScene();

    // Utility 
    void toggleFullscreen();

    // Input Callbacks
    void handleMouse(double xpos, double ypos);
    void handleScroll(double yoffset);

    // Callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};