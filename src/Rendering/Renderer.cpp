# include "Rendering/Renderer.h"

Renderer::Renderer(std::shared_ptr<Shader> shader, GLFWwindow* window)
    : shaderProgram(shader), window(window) {}

void Renderer::AddEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

void Renderer::Render(const glm::mat4& view, const glm::mat4& projection) {
    
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderProgram->Activate();

    if (entities.empty()){
        std::cout << "Warning: No entities to render.\n";
    }

    for (auto& entity : entities) {

        glm::mat4 model = entity->GetModelMatrix();
        glm::mat4 mvp = projection * view * model;


        // Assuming uniform locations or names:
        shaderProgram->SetMat4("uMVP", mvp);
        // shaderProgram->SetMat4("uModel", model);
        // Set other uniforms like normal matrix if needed

        entity->GetMesh()->Draw();
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}
