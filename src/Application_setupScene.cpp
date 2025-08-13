#include "Application.h"

void Application::setupScene()
{
    // Test Cube
    std::vector<float> cubeVerts(cubeVertices, cubeVertices + sizeof(cubeVertices)/sizeof(float));
    std::vector<unsigned int> cubeInds(cubeIndices, cubeIndices + sizeof(cubeIndices)/sizeof(unsigned int));
    auto cubeMesh = std::make_shared<Mesh>(cubeVerts, cubeInds);
    auto cube = std::make_shared<Entity>(cubeMesh);
    // SET MATERIAL UNIFORMS HERE
    renderer->AddEntity(cube);
    cube->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));


    std::vector<float> sphereVertices = generateSphereVertices(32.0f, 1.0f);
    std::vector<unsigned int> sphereIndices = generateSphereIndices(32.0f);
    auto sphereMesh = std::make_shared<Mesh>(sphereVertices, sphereIndices);
    auto sphere = std::make_shared<Entity>(sphereMesh);
    renderer->AddEntity(sphere);
    sphere->getMaterial().diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    sphere->getMaterial().specular = glm::vec3(0.1f);
    sphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}