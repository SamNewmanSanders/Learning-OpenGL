#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Rendering/Camera.h"
#include "Utils/fpsTracker.h"

class UIHandler {
public:
    UIHandler(GLFWwindow* window, FpsTracker& fpsTracker, Camera& cam);
    ~UIHandler();

    void beginFrame();  
    void render(float& deltaTimeMultiplier);       



private:
    GLFWwindow* window;
    FpsTracker& fpsTracker;
    Camera& camera;
};
