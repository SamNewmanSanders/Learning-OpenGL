#pragma once

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

#include "uiHandler.h"
#include "Entity.h"
#include "PhysicsUpdater.h"

#include "Rendering/Wrappers/Shader.h"
#include "Rendering/cubeData.h"
#include "Rendering/Wrappers/VAO.h"
#include "Rendering/Wrappers/VBO.h"
#include "Rendering/Wrappers/EBO.h"
#include "Rendering/Camera.h"
#include "Rendering/Renderer.h"

#include "Utils/Helpers.h"
#include "Utils/fpsTracker.h"
#include "Utils/performanceLogger.h"



// Declare Globals

inline int numSpheres = 5000;
inline float boxSize = 100.0f;
inline float wallThickness = 1.0f;
inline float sphereRadius = 0.5f;
inline float cubeTransparency = 0.05f;
inline float sphereSections = 8.0f;
inline float gridCellSize = 1.0f;

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
    std::unique_ptr<PhysicsUpdater> physicsUpdater;
    FpsTracker fpsTracker;
    std::unique_ptr<UIHandler> uiHandler;
    PerformanceLogger perfLogger;


    std::vector<Entity> dynamicEntities;
    std::vector<Entity> staticEntities;


    // Timing
    float deltaTime;
    float lastFrame;
    float deltaTimeMultiplier = 1.0f;

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

