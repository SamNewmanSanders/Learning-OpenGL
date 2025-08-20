#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Rendering/Wrappers/VAO.h"
#include "Rendering/Wrappers/VBO.h"
#include "Rendering/Wrappers/EBO.h"


class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);
    ~Mesh();

    void setUpInstancing(size_t maxNumInstances);

    void Draw();  // draw the mesh (single instance for now)

    void updateInstanceMatrices(const std::vector<glm::mat4>& models);
    void updateInstanceDiffuseColors(const std::vector<glm::vec3>& colors);
    void DrawInstanced(GLsizei count);

private:

    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;   
    std::unique_ptr<EBO> ebo;
    size_t indexCount; // For ebo

    std::unique_ptr<VBO> instancevbo; // stores per-instance model matrices
    std::unique_ptr<VBO> instanceColorvbo; 
    size_t maxInstances = 0; // Max number of spheres (in this case)

};