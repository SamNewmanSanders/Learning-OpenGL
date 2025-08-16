#include "Application.h"

void Application::update() {
    
    fpsTracker.update((float)glfwGetTime());

    // Bouncing logic
    const float bounds = boxSize/2 - sphereRadius - wallThickness/2 ; // Sphere radius and wall thickness

    float simDeltaTime = deltaTime * deltaTimeMultiplier;

    // First pass: apply gravity + wall bounces
    for (auto& entity : dynamicEntities) {
        glm::vec3 pos = entity.getPosition();
        glm::vec3 vel = entity.getVelocity();

        // Gravity
        vel += glm::vec3(0.0f, -9.81f, 0.0f) * simDeltaTime;
        pos += vel * simDeltaTime;

        // Bounce against boundaries
        if (pos.y < -bounds) { pos.y = -bounds; vel.y *= -1.0f; }
        if (pos.x < -bounds) { pos.x = -bounds; vel.x *= -1.0f; }
        if (pos.x >  bounds) { pos.x =  bounds; vel.x *= -1.0f; }
        if (pos.z < -bounds) { pos.z = -bounds; vel.z *= -1.0f; }
        if (pos.z >  bounds) { pos.z =  bounds; vel.z *= -1.0f; }

        entity.setPosition(pos);
        entity.setVelocity(vel);
    }

    // Second pass: ball-ball collisions
    for (size_t i = 0; i < dynamicEntities.size(); ++i) {
        for (size_t j = i + 1; j < dynamicEntities.size(); ++j) {
            auto& a = dynamicEntities[i];
            auto& b = dynamicEntities[j];

            glm::vec3 posA = a.getPosition();
            glm::vec3 posB = b.getPosition();
            glm::vec3 velA = a.getVelocity();
            glm::vec3 velB = b.getVelocity();
            float rA = sphereRadius;
            float rB = sphereRadius;

            glm::vec3 delta = posB - posA;
            float dist = glm::length(delta);
            float minDist = rA + rB;

            if (dist < minDist && dist > 0.0f) {
                glm::vec3 normal = delta / dist;

                // Relative velocity (B relative to A)
                glm::vec3 relVel = velB - velA;
                float velAlongNormal = glm::dot(relVel, normal);

                if (velAlongNormal < 0.0f) {
                    float restitution = 1.0f;
                    float invMassA = 1.0f;
                    float invMassB = 1.0f;

                    float impulseMag = -(1.0f + restitution) * velAlongNormal
                                    / (invMassA + invMassB);

                    glm::vec3 impulse = impulseMag * normal;
                    velA -= impulse * invMassA;
                    velB += impulse * invMassB;
                }

                // Positional correction
                float penetration = minDist - dist;
                glm::vec3 correction = normal * (penetration * 0.5f);
                posA -= correction;
                posB += correction;

                // Write back
                a.setPosition(posA);
                b.setPosition(posB);
                a.setVelocity(velA);
                b.setVelocity(velB);
            }
        }
    }
}