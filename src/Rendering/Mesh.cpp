#include "Rendering/Mesh.h"


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

    // For now only care about position attribute (NOT NORMAL OR TEXTURE - STILL KEEP STRIDE AS 8)
    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);

    vbo->Unbind();
    ebo->Unbind();
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
