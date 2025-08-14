#include "Application.h"
#include <random>
#include <iostream>

// Excuse the bodged code but I find this setup stuff really boring to do just got chatgpt to do it

void Application::setupScene()
{
    float boxSize = 50.0f;    // Internal width/length/height of box
    float wallThickness = 1.0f;
    float sphereRadius = 0.5f;
    float halfBox = boxSize / 2.0f;
    float halfWall = wallThickness / 2.0f;

    float cubeTransparency = 0.2f;

    std::vector<float> cubeVerts(cubeVertices, cubeVertices + sizeof(cubeVertices)/sizeof(float));
    std::vector<unsigned int> cubeInds(cubeIndices, cubeIndices + sizeof(cubeIndices)/sizeof(unsigned int));
    auto cubeMesh = std::make_shared<Mesh>(cubeVerts, cubeInds);

    // Bottom
    auto bottom = Entity(cubeMesh);
    bottom.setScale(glm::vec3(boxSize, wallThickness, boxSize));
    bottom.setPosition(glm::vec3(0.0f, -halfBox - halfWall, 0.0f));    
    bottom.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f); 
    bottom.getMaterial().alpha = cubeTransparency; 
    staticEntities.push_back(bottom); // THIS HAS TO BE DONE LAST AS IT IS A COPY

    // Back wall
    auto back = Entity(cubeMesh);
    back.setScale(glm::vec3(boxSize, boxSize, wallThickness));
    back.setPosition(glm::vec3(0.0f, 0.0f, -halfBox - halfWall));
    back.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f); 
    back.getMaterial().alpha = cubeTransparency; 
    staticEntities.push_back(back);

    // Front wall
    auto front = Entity(cubeMesh);
    front.setScale(glm::vec3(boxSize, boxSize, wallThickness));
    front.setPosition(glm::vec3(0.0f, 0.0f, halfBox + halfWall));
    front.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f); 
    front.getMaterial().alpha = cubeTransparency; 
    staticEntities.push_back(front);

    // Left wall
    auto left = Entity(cubeMesh);
    left.setScale(glm::vec3(wallThickness, boxSize, boxSize));
    left.setPosition(glm::vec3(-halfBox - halfWall, 0.0f, 0.0f));
    left.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f); 
    left.getMaterial().alpha = cubeTransparency; 
    staticEntities.push_back(left);

    // Right wall
    auto right = Entity(cubeMesh);
    right.setScale(glm::vec3(wallThickness, boxSize, boxSize));
    right.setPosition(glm::vec3(halfBox + halfWall, 0.0f, 0.0f));
    right.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f); 
    right.getMaterial().alpha = cubeTransparency; 
    staticEntities.push_back(right);

    // Prepare sphere mesh once (reuse for all spheres)
    std::vector<float> sphereVertices = generateSphereVertices(15.0f, sphereRadius);
    std::vector<unsigned int> sphereIndices = generateSphereIndices(15.0f);
    auto sphereMesh = std::make_shared<Mesh>(sphereVertices, sphereIndices);

    // Random engine setup
    std::random_device rd;
    std::mt19937 gen(rd());

    // Position distributions (inside box accounting for radius and wall thickness)
    float halfBoxnew = boxSize / 2.0f - wallThickness / 2.0f - sphereRadius;
    std::uniform_real_distribution<float> distX(-halfBoxnew, halfBoxnew);
    std::uniform_real_distribution<float> distY(-halfBoxnew, halfBoxnew);
    std::uniform_real_distribution<float> distZ(-halfBoxnew, halfBoxnew);

    std::uniform_real_distribution<float> velX(-halfBoxnew, halfBoxnew);
    //std::uniform_real_distribution<float> velY(-halfBoxnew, halfBoxnew);
    std::uniform_real_distribution<float> velZ(-halfBoxnew, halfBoxnew);

    // Color distributions
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

    for (int i = 0; i < 1000; ++i) {
        auto sphere = Entity(sphereMesh);

        // Random position within bounds
        glm::vec3 randomPos(distX(gen), distY(gen), distZ(gen));
        sphere.setPosition(randomPos);

        // Random diffuse color
        glm::vec3 randomColor(distColor(gen), distColor(gen), distColor(gen));
        sphere.getMaterial().diffuse = randomColor;

        // Specular & shininess
        sphere.getMaterial().specular = glm::vec3(0.1f);
        sphere.getMaterial().shininess = 8.0f;

        // Initialize velocity
        sphere.setVelocity(glm::vec3(velX(gen), 0.0f, velZ(gen)));

        dynamicEntities.push_back(sphere);

    }
}