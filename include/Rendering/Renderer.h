#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "Rendering/Wrappers/Shader.h"
#include "Rendering/Lighting/Light.h"
#include "Rendering/Camera.h"   
#include "Entity.h"



class Renderer {
public:
    Renderer(std::shared_ptr<Shader> shader, GLFWwindow* window);

    void beginFrame();

    void drawEntity(Entity& entity, 
        const Camera& camera);

    void drawInstancedEntities(std::vector<Entity>& dynamicEntities, const Camera& camera);

    void endFrame();

private:
    std::shared_ptr<Shader> shaderProgram;
    GLFWwindow* window;
    Light lightSource;
};

