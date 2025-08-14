# include "Rendering/Renderer.h"

Renderer::Renderer(std::shared_ptr<Shader> shader, GLFWwindow* window)
    : shaderProgram(shader), window(window) 
    {
        // Add light source
        lightSource = { glm::vec3(10.0f, -10.0f, -10.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f };
    }

void Renderer::beginFrame() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->Activate();
}

void Renderer::drawEntity(const std::shared_ptr<Entity>& entity,
    const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& viewPos) {

    glm::mat4 model = entity->getModelMatrix();

    shaderProgram->setVec3("material.diffuse", entity->getMaterial().diffuse);
    shaderProgram->setVec3("material.specular", entity->getMaterial().specular);
    shaderProgram->setFloat("material.shininess", entity->getMaterial().shininess);

    shaderProgram->setVec3("light.position", lightSource.position);
    shaderProgram->setVec3("light.color", lightSource.color);
    shaderProgram->setFloat("light.intensity", lightSource.intensity);

    shaderProgram->setVec3("viewPos", viewPos);

    shaderProgram->setMat4("model", model);
    shaderProgram->setMat4("view", view);
    shaderProgram->setMat4("projection", projection);

    shaderProgram->setFloat("uAlpha", entity->getMaterial().alpha);

    entity->getMesh()->Draw();
    
}

void Renderer::endFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}
