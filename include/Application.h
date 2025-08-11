#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

#include "Shader.h"
#include "cubeData.h"
#include "VAO.h"
#include "VBO.h"
#include "Camera.h"


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


    // Set the following as pointers that are defaulted to nullptr
    // This avoids issues with their constructors being called before OpenGL context is ready
    
    std::unique_ptr<Shader> shaderProgram;
    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;

    float deltaTime;
    float lastFrame;

    GLint mvpLoc;
};