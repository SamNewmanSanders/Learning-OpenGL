#include "Application.h"


// TEMPORARY GLOBALS - consider better mouse handling later
static bool firstMouse = true;
static float lastX = 400.0f;  // Initial mouse position (center of window)
static float lastY = 300.0f;

// Forward declare callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Forward declare opengl boilerplate
GLFWwindow* initWindowAndGL(int width, int height, const char* title, Camera& camera);

Application::Application(int width, int height, const char* title)
    : window(nullptr),
      camera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), -90.0f, 0.0f),
      deltaTime(0.0f),
      lastFrame(0.0f)
{
    // Boilerplate initialization
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
}

void Application::update() {
    
    fpsFrames++;
    float currentTime = (float)glfwGetTime();
    float delta = currentTime - fpsTimer;

    if (delta >= 1.0f) {
        fps = (float)fpsFrames / delta;
        fpsFrames = 0;
        fpsTimer = currentTime;

        // Optionally print FPS to console:
        std::cout << "FPS: " << fps << std::endl;
    }
}

void Application::render() {

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    float aspect = (w > 0 && h > 0) ? (float)w / (float)h : 4.0f / 3.0f;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 100.0f);

    glm::vec3 viewPos = camera.getPosition();

    renderer->Render(view, proj, viewPos);
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

    return window;
}

