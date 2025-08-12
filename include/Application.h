#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

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

    void processInputs();
    void update();
    void render();

    GLFWwindow* window;
    Camera camera;
    std::unique_ptr<Renderer> renderer;

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    float deltaTime;
    float lastFrame;


    // Information
    float fps = 0.0f;
    float fpsTimer = 0.0f;
    int fpsFrames = 0;
};