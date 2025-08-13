#include "Rendering/Mesh.h"

#include <iostream>


Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : indexCount(indices.size())
{
    // Create VAO, VBO, and EBO instances
    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = std::make_unique<EBO>(indices.data(), indices.size() * sizeof(GLuint));

    vao->Bind();
    vbo->Bind();
    ebo->Bind();    

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);                   // position
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float))); // normal

    vbo->Unbind();
    vao->Unbind();
}

Mesh::~Mesh() {
    vbo->Delete();
    vao->Delete();
    ebo->Delete();
}

void Mesh::Draw() {
    vao->Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    vao->Unbind();
}
