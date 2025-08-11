#include "Application.h"

// TEMPORARY GLOBALS - consider better mouse handling later
static bool firstMouse = true;
static float lastX = 400.0f;  // Initial mouse position (center of window)
static float lastY = 300.0f;

// Forward declare callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Application::Application(int width, int height, const char* title)
    : window(nullptr),
      camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
      deltaTime(0.0f),
      lastFrame(0.0f)
{
    // Init GLFW
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    // Initialize shader AFTER GLAD and context are ready
    shaderProgram = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mvpLoc = glGetUniformLocation(shaderProgram->ID, "uMVP");

    sphereVertices = generateSphereVertices(32, 1.0f);
    sphereIndices = generateSphereIndices(32);

    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(sphereVertices.data(), sphereVertices.size() * sizeof(float));
    ebo = std::make_unique<EBO>(sphereIndices.data(), sphereIndices.size() * sizeof(unsigned int));

    // Setup VBO and VAO attributes
    vao->Bind();
    vbo->Bind();
    ebo->Bind();

    // Position attribute
    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);

    // Normal attribute
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // UV attribute
    vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();

}

Application::~Application() {
    vbo->Delete();
    vao->Delete();
    // unique_ptr will auto-delete shaderProgram
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
    // Future update logic here
}

void Application::render() {
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    float aspect = (w > 0 && h > 0) ? (float)w / (float)h : 4.0f / 3.0f;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 100.0f);

    glm::mat4 mvp = proj * view * model;


    shaderProgram->Activate();
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    vao->Bind();
    // Number of indices to draw:
    GLsizei indexCount = static_cast<GLsizei>(sphereIndices.size());
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Callbacks

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
