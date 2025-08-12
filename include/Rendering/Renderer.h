#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "Rendering/Wrappers/Shader.h"
#include "Entity.h"



class Renderer {
public:
    Renderer(std::shared_ptr<Shader> shader, GLFWwindow* window);

    void AddEntity(std::shared_ptr<Entity> entity);
    void Render(const glm::mat4& view, const glm::mat4& projection);

private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::shared_ptr<Shader> shaderProgram;
    GLFWwindow* window;
};

