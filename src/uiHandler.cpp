#include "UIHandler.h"
#include <iostream>

UIHandler::UIHandler(GLFWwindow* window, FpsTracker& fpsTracker, Camera& cam)
    : window(window), fpsTracker(fpsTracker), camera(cam)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UIHandler::~UIHandler() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIHandler::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIHandler::render(float& deltaTimeMultiplier) {

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

    // FPS
    ImGui::Text("FPS: %.1f", fpsTracker.getFps());

    // Camera coordinates
    glm::vec3 camPos = camera.getPosition();
    ImGui::Text("Camera X: %.2f", camPos.x);
    ImGui::Text("Camera Y: %.2f", camPos.y);
    ImGui::Text("Camera Z: %.2f", camPos.z);

    // Slider for someValue
    ImGui::SliderFloat("Sim Speed", &deltaTimeMultiplier, 0.0f, 1.0f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

