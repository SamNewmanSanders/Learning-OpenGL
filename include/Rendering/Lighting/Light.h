#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position; // or direction for directional lights
    glm::vec3 color;
    float intensity;
};
