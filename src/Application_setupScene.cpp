#include "Application.h"

#include <random>

void Application::setupScene()
{
    std::vector<float> cubeVerts(cubeVertices, cubeVertices + sizeof(cubeVertices)/sizeof(float));
    std::vector<unsigned int> cubeInds(cubeIndices, cubeIndices + sizeof(cubeIndices)/sizeof(unsigned int));
    auto cubeMesh = std::make_shared<Mesh>(cubeVerts, cubeInds);
    auto cube = std::make_shared<Entity>(cubeMesh);
    staticEntities.push_back(cube);
    cube->setPosition(glm::vec3(0.0f, -25.5f, 0.0f)); // Top face at y = -25.0
    cube->setScale(glm::vec3(50.0f, 1.0f, 50.0f));

    // Prepare sphere mesh once (reuse for all spheres)
    std::vector<float> sphereVertices = generateSphereVertices(15.0f, 0.5f);
    std::vector<unsigned int> sphereIndices = generateSphereIndices(15.0f);
    auto sphereMesh = std::make_shared<Mesh>(sphereVertices, sphereIndices);

    // Random engine setup
    std::random_device rd;
    std::mt19937 gen(rd());

    // Position distributions
    std::uniform_real_distribution<float> distX(-25.0f, 25.0f);
    std::uniform_real_distribution<float> distY(-25.0f, 25.0f);
    std::uniform_real_distribution<float> distZ(-25.0f, 25.0f);

    // Color distributions
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

    for (int i = 0; i < 100; ++i) {
        auto sphere = std::make_shared<Entity>(sphereMesh);
        dynamicEntities.push_back(sphere);

        // Random position within bounds
        glm::vec3 randomPos(distX(gen), distY(gen), distZ(gen));
        sphere->setPosition(randomPos);

        // Random diffuse color
        glm::vec3 randomColor(distColor(gen), distColor(gen), distColor(gen));
        sphere->getMaterial().diffuse = randomColor;

        // You can keep specular fixed or randomize too
        sphere->getMaterial().specular = glm::vec3(0.1f);
        sphere->getMaterial().shininess = 8.0f;  // or randomize if you want
    }
}
