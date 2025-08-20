#include "Rendering/Mesh.h"

#include <iostream>


Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : indexCount(indices.size())
{
    // Create VAO, VBO, and EBO
    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = std::make_unique<EBO>(indices.data(), indices.size() * sizeof(GLuint));

    vao->Bind();
    vbo->Bind();
    ebo->Bind();    

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);                   // position
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float))); // normal

    vbo->Unbind();


    // Set up instancing stuff

    // Allocate buffer for all instances at once
    instancevbo = std::make_unique<VBO>(nullptr, maxInstances * sizeof(glm::mat4));
    instancevbo->Bind(); // As long as the correct vbo is bound it will remember the config (can use same vbo)

    const GLuint loc = 2;       // First attribute slot for the mat4
    const GLsizei vec4Size = sizeof(glm::vec4); // 16 bytes
    const GLsizei stride   = sizeof(glm::mat4); // 64 bytes

    for (int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(loc + i);
        glVertexAttribPointer(
            loc + i,
            4, GL_FLOAT, GL_FALSE,
            stride,
            reinterpret_cast<const void*>(static_cast<size_t>(i) * vec4Size)
        );
        glVertexAttribDivisor(loc + i, 1); // use new value per instance, not per vertex
    }

    instancevbo->Unbind();

    instanceColorvbo = std::make_unique<VBO>(nullptr, maxInstances * sizeof(glm::vec3));

    instanceColorvbo->Bind();

    const GLuint locColor = 6; // must match shader layout(location = 6)
    glEnableVertexAttribArray(locColor);
    glVertexAttribPointer(locColor, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(locColor, 1); // advances per instance
    
    instanceColorvbo->Unbind();
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

void Mesh::updateInstanceMatrices(const std::vector<glm::mat4>& models) {
    instancevbo->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, models.size() * sizeof(glm::mat4), models.data());
    instancevbo->Unbind();
}

void Mesh::DrawInstanced() {
    vao->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0, maxInstances);
    vao->Unbind();
}

void Mesh::updateInstanceDiffuseColors(const std::vector<glm::vec3>& colors) {

    // Update existing buffer
    instanceColorvbo->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec3), colors.data());
    instanceColorvbo->Unbind();

}
