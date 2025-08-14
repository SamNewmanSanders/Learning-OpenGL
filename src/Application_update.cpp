#include "Application.h"

void Application::update() {
    fpsFrames++;
    float currentTime = (float)glfwGetTime();
    float delta = currentTime - fpsTimer;

    if (delta >= 1.0f) {
        fps = (float)fpsFrames / delta;
        fpsFrames = 0;
        fpsTimer = currentTime;
        std::cout << "FPS: " << fps << std::endl;
    }

    // Bouncing logic
    const float bounds = 25.0f -0.5f; // Sphere radius - make this automatic at some point
    for (auto& entity : dynamicEntities) {
        glm::vec3 pos = entity->getPosition();
        glm::vec3 vel = entity->getVelocity();

        vel+= glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime; // Gravity effect
        pos += vel * deltaTime;

        // Bounce check
        if (pos.y < -bounds) { pos.y = -bounds; vel.y *= -1.0f; }

        entity->setPosition(pos);
        entity->setVelocity(vel);
    }
}