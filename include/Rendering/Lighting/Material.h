#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    float alpha; // Transparency

    // Create a default material

    Material()
    : diffuse(1.0f, 1.0f, 1.0f)    
    , specular(1.0f, 1.0f, 1.0f)   
    , shininess(8.0f)
    , alpha(1.0f) {}            
};