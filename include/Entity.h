#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Rendering/Mesh.h"
#include "Rendering/Lighting/Material.h"


class Entity {
public:
    Entity(std::shared_ptr<Mesh> mesh);

    void setPosition(const glm::vec3& pos);
    const glm::vec3& getPosition() const { return position; }
    void setRotation(const glm::vec3& rot);  // or use quaternion
    void setScale(const glm::vec3& scale);

    glm::mat4 getModelMatrix() const;

    std::shared_ptr<Mesh> getMesh() const;

    Material& getMaterial() { return material; } // Different things call this differently so provide two functions
    const Material& getMaterial() const { return material; }

    void setVelocity(const glm::vec3& vel) { velocity = vel; }
    glm::vec3& getVelocity() { return velocity; }

private:
    std::shared_ptr<Mesh> mesh;
    Material material; 

    glm::vec3 position;
    glm::vec3 rotation;  // e.g. Euler angles in degrees
    glm::vec3 scale;

    glm::vec3 velocity;
};