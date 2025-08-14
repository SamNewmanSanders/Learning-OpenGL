#include "Application.h"

// TEMPORARY GLOBALS - consider better mouse handling later
static bool firstMouse = true;
static float lastX = 600.0f;  // Initial mouse position (center of window)
static float lastY = 400.0f;

// Forward declare callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Forward declare opengl boilerplate
GLFWwindow* initWindowAndGL(int width, int height, const char* title, Camera& camera);

Application::Application(int width, int height, const char* title)
    : window(nullptr),
      camera(glm::vec3(-60, 35, -60), glm::vec3(0, 1, 0), 45.0f, -30.0f), // Best view for me
      deltaTime(0.0f),
      lastFrame(0.0f)
{
    // Use boilerplate initialization function
    window = initWindowAndGL(width, height, title, camera);

    // Initialize shader AFTER GLAD and context are ready    
    std::shared_ptr<Shader> shaderProgram = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");

    // Assign Renderer
    renderer = std::make_unique<Renderer>(shaderProgram, window);
    
    //Set up scene
    setupScene();
}

Application::~Application() {
    
    // Renderer should delete itself

    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

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

void Application::processInputs() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.processKeyboard("FORWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.processKeyboard("BACKWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.processKeyboard("LEFT", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.processKeyboard("RIGHT", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        camera.processKeyboard("UPWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        camera.processKeyboard("DOWNWARD", deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        static bool pressed = false;
        if (!pressed) {
            toggleFullscreen();
            pressed = true;
        }       else {
            pressed = false;
        }
    }
}


void Application::render() {
    
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Draw UI
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
    ImGui::Text("Camera X: %.2f", camera.getPosition().x);
    ImGui::End();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    float aspect = (w > 0 && h > 0) ? (float)w / (float)h : 4.0f / 3.0f;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 200.0f);

    glm::vec3 viewPos = camera.getPosition();

    renderer->beginFrame();
    for (const auto& entity : dynamicEntities) {
        renderer->drawEntity(entity, view, proj, viewPos);
    }
    
    // Use helper functions to sort by distance and enable transparency
    auto sortedStatics = sortEntitiesByDistance(staticEntities, camera.getPosition());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // stop writing to depth buffer
    
    for (const auto& entity : sortedStatics) {
        renderer->drawEntity(entity, view, proj, viewPos);
    }

    //Reenable depth testing
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    renderer->endFrame();

    ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::toggleFullscreen() {
    if (!isFullscreen) {
        // Save windowed position and size
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

        // Get primary monitor and its video mode
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Switch to fullscreen
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    } else {
        // Restore to windowed mode
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
        isFullscreen = false;
    }
}

// Callbacks ---------------------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    cam->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    cam->processMouseScroll((float)yoffset);
}


// OpenGL Boilerplate ------------------------------------------------------------------------------------------------

GLFWwindow* initWindowAndGL(int width, int height, const char* title, Camera& camera) {
    
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);                       // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Set up ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Optional
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Optional

    ImGui::StyleColorsDark(); // Or StyleColorsClassic()

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // true = let ImGui install its own callbacks
    ImGui_ImplOpenGL3_Init("#version 330");

    return window;
}

