#include "Application.h"

// === Constructor / Destructor ===
Application::Application(int width, int height, const char* title)
    : window(nullptr),
      camera(glm::vec3(-60, 35, -60), glm::vec3(0, 1, 0), 45.0f, -30.0f),
      deltaTime(0.0f),
      lastFrame(0.0f)
{
    initWindow(width, height, title);
    initOpenGL();
    initImGui();

    auto shaderProgram = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
    renderer = std::make_unique<Renderer>(shaderProgram, window);

    setupScene();
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

// === Init Functions ===
void Application::initWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Store the application class in the GLWF window
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void Application::initOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

// === Main Loop ===
void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInputs();
        update();
        render();
    }
}

// === Loop Steps ===
void Application::processInputs() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.processKeyboard("FORWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.processKeyboard("BACKWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.processKeyboard("LEFT", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.processKeyboard("RIGHT", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) camera.processKeyboard("UPWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) camera.processKeyboard("DOWNWARD", deltaTime);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        static bool pressed = false; // Acts like a debouncer
        if (!pressed) {
            toggleFullscreen();
            pressed = true;
        }
    } else {
        static bool pressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!input.tabPressedLastFrame) {
            input.mouseEnabled = !input.mouseEnabled;
            if (input.mouseEnabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                input.firstMouse = true;
            }
            input.tabPressedLastFrame = true;
        }
    } else {
        input.tabPressedLastFrame = false;
    }
}

void Application::render() {
    renderUI();
    renderScene();
}

// === Rendering Helpers ===
void Application::renderUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
    ImGui::Text("Camera X: %.2f", camera.getPosition().x);
    ImGui::End();
}

void Application::renderScene() {

    renderer->beginFrame();

    for (auto& entity : dynamicEntities)
        renderer->drawEntity(entity, camera);

    auto sortedStatics = sortEntitiesByDistance(staticEntities, camera.getPosition());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // stop writing to depth buffer

    for (auto& entity : sortedStatics)
        renderer->drawEntity(entity, camera);

    //Reenable depth testing
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    renderer->endFrame();
}

// === Utility ===
void Application::toggleFullscreen() {
    if (!isFullscreen) {
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    } else {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
        isFullscreen = false;
    }
}

// === Input Handlers ===
void Application::handleMouse(double xpos, double ypos) {
    if (!input.mouseEnabled) {
        if (input.firstMouse) {
            input.lastX = (float)xpos;
            input.lastY = (float)ypos;
            input.firstMouse = false;
        }
        float xoffset = (float)xpos - input.lastX;
        float yoffset = input.lastY - (float)ypos;
        input.lastX = (float)xpos;
        input.lastY = (float)ypos;
        camera.processMouseMovement(xoffset, yoffset);
    }
}

void Application::handleScroll(double yoffset) {
    camera.processMouseScroll((float)yoffset);
}

// === Static Callbacks ===
void Application::framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->handleMouse(xpos, ypos);
}

void Application::scrollCallback(GLFWwindow* window, double, double yoffset) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->handleScroll(yoffset);
}
