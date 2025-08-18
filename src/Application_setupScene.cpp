#include "Application.h"
#include <random>
#include <iostream>

#include "Application.h"
#include <random>
#include <iostream>

void Application::setupScene()
{

    auto cubeMesh = std::make_shared<Mesh>(
        std::vector<float>(cubeVertices, cubeVertices + sizeof(cubeVertices)/sizeof(float)),
        std::vector<unsigned int>(cubeIndices, cubeIndices + sizeof(cubeIndices)/sizeof(unsigned int))
    );

    // I finally understand lambdas! So you can capture local variables
    auto addWall = [&](const glm::vec3& pos, const glm::vec3& scale) {
        staticEntities.emplace_back(cubeMesh);
        auto& wall = staticEntities.back();
        wall.setPosition(pos);
        wall.setScale(scale);
        wall.getMaterial().diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
        wall.getMaterial().alpha = cubeTransparency;
    };

    float halfBox = boxSize / 2.0f;
    float halfWall = wallThickness / 2.0f;

    // Add all walls
    addWall(glm::vec3(0.0f, -halfBox - halfWall, 0.0f), glm::vec3(boxSize, wallThickness, boxSize)); // bottom
    addWall(glm::vec3(0.0f, 0.0f, -halfBox - halfWall), glm::vec3(boxSize, boxSize, wallThickness));   // back
    addWall(glm::vec3(0.0f, 0.0f, halfBox + halfWall), glm::vec3(boxSize, boxSize, wallThickness));    // front
    addWall(glm::vec3(-halfBox - halfWall, 0.0f, 0.0f), glm::vec3(wallThickness, boxSize, boxSize));  // left
    addWall(glm::vec3(halfBox + halfWall, 0.0f, 0.0f), glm::vec3(wallThickness, boxSize, boxSize));   // right
    addWall(glm::vec3(0.0f, halfBox + halfWall, 0.0f), glm::vec3(boxSize, wallThickness, boxSize)); // top



    // Sphere setup
    auto sphereMesh = std::make_shared<Mesh>(
        generateSphereVertices(sphereSections, sphereRadius),
        generateSphereIndices(sphereSections)
    );

    std::random_device rd;
    std::mt19937 gen(rd());
    float halfBoxInner = boxSize / 2.0f - wallThickness / 2.0f - sphereRadius;

    std::uniform_real_distribution<float> distPos(-halfBoxInner, halfBoxInner);
    std::uniform_real_distribution<float> distVel(-halfBoxInner, halfBoxInner);
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

    for (int i = 0; i < numSpheres; ++i) {
        dynamicEntities.emplace_back(sphereMesh);
        auto& sphere = dynamicEntities.back();

        sphere.setPosition(glm::vec3(distPos(gen), distPos(gen), distPos(gen)));
        sphere.getMaterial().diffuse = glm::vec3(distColor(gen), distColor(gen), distColor(gen));
        sphere.getMaterial().specular = glm::vec3(0.1f);
        sphere.getMaterial().shininess = 8.0f;
        sphere.setVelocity(glm::vec3(distVel(gen), 0.0f, distVel(gen)));
    }
}
