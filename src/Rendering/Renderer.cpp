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

void Renderer::drawEntity(Entity& entity,
    const Camera& camera) {

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    float aspect = (w > 0 && h > 0) ? (float)w / (float)h : 4.0f / 3.0f;

    glm::mat4 model = entity.getModelMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 500.0f);
    glm::vec3 viewPos = camera.getPosition();

    shaderProgram->setBool("useInstancing", false); // Non-instance draw call

    shaderProgram->setVec3("specularColor", entity.getMaterial().specular); // Use uniform for non instance call
    shaderProgram->setFloat("Shininess", entity.getMaterial().shininess);
    shaderProgram->setVec3("uniformDiffuseColor", entity.getMaterial().diffuse); // Got rid of material struct for now
    shaderProgram->setFloat("uAlpha", entity.getMaterial().alpha);

    shaderProgram->setVec3("light.position", lightSource.position);
    shaderProgram->setVec3("light.color", lightSource.color);
    shaderProgram->setFloat("light.intensity", lightSource.intensity);

    shaderProgram->setVec3("viewPos", viewPos);
    shaderProgram->setMat4("view", view);
    shaderProgram->setMat4("projection", projection);

    shaderProgram->setMat4("model", model); // For non instance call

    entity.getMesh()->Draw();
}

void Renderer::drawInstancedEntities(std::vector<Entity>& dynamicEntities, const Camera& camera)
{
    if (dynamicEntities.empty()){
        std::cout<<"No dynamic entities to render\n";
        return;
    }

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    float aspect = (w > 0 && h > 0) ? (float)w / (float)h : 4.0f / 3.0f;

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 500.0f);
    glm::vec3 viewPos = camera.getPosition();

    shaderProgram->setBool("useInstancing", true); // Instance draw call

    // Set uniforms that don't change between entities
    shaderProgram->setVec3("light.position", lightSource.position);
    shaderProgram->setVec3("light.color", lightSource.color);
    shaderProgram->setFloat("light.intensity", lightSource.intensity);
    shaderProgram->setMat4("view", view);
    shaderProgram->setMat4("projection", projection);
    shaderProgram->setVec3("viewPos", viewPos);
    shaderProgram->setVec3("specularColor", dynamicEntities[0].getMaterial().specular); // Using the first index is messy but fine for now
    shaderProgram->setFloat("Shininess", dynamicEntities[0].getMaterial().shininess);
    shaderProgram->setFloat("uAlpha", dynamicEntities[0].getMaterial().alpha);

    std::vector<glm::mat4> models; // Vector of model matrices
    models.reserve(dynamicEntities.size()); // Pre allocate the memory to avoid expensive copy operations - play with removing this line

    // For now only change the diffuseColors AS THAT IS THE ONLY THING I CHANGE IN SETUP - DON'T FORGET THIS
    std::vector<glm::vec3> diffuseColors; 
    diffuseColors.reserve(dynamicEntities.size());

    for (const auto& e : dynamicEntities) {
        models.push_back(e.getModelMatrix());
        diffuseColors.push_back(e.getMaterial().diffuse);
    }

    // Upload per-instance data to the mesh
    std::shared_ptr<Mesh> mesh = dynamicEntities[0].getMesh(); // they all share one mesh
    
    mesh->updateInstanceMatrices(models);
    mesh->updateInstanceDiffuseColors(diffuseColors);

    // --- 3) Draw all instances in one call ---
    mesh->DrawInstanced();
}




void Renderer::endFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}
