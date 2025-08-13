# include "Rendering/Renderer.h"

Renderer::Renderer(std::shared_ptr<Shader> shader, GLFWwindow* window)
    : shaderProgram(shader), window(window) 
    {
        // Add light source
        lightSource = { glm::vec3(10.0f, 10.0f, 10.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f };
    }

void Renderer::AddEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

void Renderer::Render(const glm::mat4& view, const glm::mat4& projection) {
    
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderProgram->Activate();

    if (entities.empty()){
        std::cout << "Warning: No entities to render.\n";
    }

    for (auto& entity : entities) {


        glm::mat4 model = entity->GetModelMatrix();

        shaderProgram->setVec3("material.diffuse", entity->getMaterial().diffuse);
        shaderProgram->setVec3("material.specular", entity->getMaterial().specular);
        shaderProgram->setFloat("material.shininess", entity->getMaterial().shininess);

        shaderProgram->setVec3("light.position", lightSource.position);
        shaderProgram->setVec3("light.color", lightSource.color);
        shaderProgram->setFloat("light.intensity", lightSource.intensity);

        // shaderProgram->setVec3("viewPos", viewPos);

        shaderProgram->setMat4("model", model);
        shaderProgram->setMat4("view", view);
        shaderProgram->setMat4("projection", projection);

        entity->GetMesh()->Draw();
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}
