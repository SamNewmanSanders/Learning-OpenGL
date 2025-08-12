#pragma once

#include <glad/glad.h>

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

    void Draw();  // draw the mesh (single instance for now)

private:

    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;   
    std::unique_ptr<EBO> ebo;

    size_t indexCount;
};