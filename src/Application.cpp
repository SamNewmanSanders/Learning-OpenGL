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
    
    // Initialize imgui with uihandler class
    uiHandler = std::make_unique<UIHandler>(window, fpsTracker, camera);

    auto shaderProgram = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
    renderer = std::make_unique<Renderer>(shaderProgram, window);
    physicsUpdater= std::make_unique<PhysicsUpdater>(boxSize, sphereRadius, wallThickness, gridCellSize);

    setupScene();
}

Application::~Application() {

    // Print report
    float averageFPS = fpsTracker.getAverageFps();
    perfLogger.report(averageFPS);

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

    // Store the application class in the GLWF window. This way can access everything it wants
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


// === Main Loop ===
void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;
        float simDeltaTime = deltaTime * deltaTimeMultiplier;

        fpsTracker.update((float)glfwGetTime());

        processInputs();

        perfLogger.beginStage("Update");
        physicsUpdater->update(simDeltaTime, dynamicEntities);
        perfLogger.endStage("Update");

        perfLogger.beginStage("Render");
        render();
        perfLogger.endStage("Render");
    }
}

// === Loop Steps ===
void Application::processInputs() {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
    }


    // Use a lambda function to handle camera movement
    auto handleCameraKey = [&](int key, const char* dir) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            camera.processKeyboard(dir, deltaTime);
    };

    // Call the lambda for each key
    handleCameraKey(GLFW_KEY_UP, "FORWARD");
    handleCameraKey(GLFW_KEY_DOWN, "BACKWARD");
    handleCameraKey(GLFW_KEY_LEFT, "LEFT");
    handleCameraKey(GLFW_KEY_RIGHT, "RIGHT");
    handleCameraKey(GLFW_KEY_1, "UPWARD");
    handleCameraKey(GLFW_KEY_2, "DOWNWARD");

    // Fullscreen toggle with proper debouncing
    static bool fPressedLastFrame = false;
    bool fPressedNow = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

    if (fPressedNow && !fPressedLastFrame) {
        toggleFullscreen();
    }
    fPressedLastFrame = fPressedNow;

    // Mouse toggle with Tab key
    bool tabPressedNow = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
    if (tabPressedNow && !input.tabPressedLastFrame) {
        input.mouseEnabled = !input.mouseEnabled;
        glfwSetInputMode(window,
                         GLFW_CURSOR,
                         input.mouseEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        if (!input.mouseEnabled) input.firstMouse = true;

        input.tabPressedLastFrame = true;
    } else if (!tabPressedNow) {
        input.tabPressedLastFrame = false;
    }
}

void Application::render() {

    uiHandler->beginFrame();

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

    uiHandler->render(deltaTimeMultiplier);

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
