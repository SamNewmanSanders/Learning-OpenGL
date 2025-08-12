#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> meshPtr)
    : mesh(meshPtr), position(0.0f), rotation(0.0f), scale(1.0f) {}

void Entity::SetPosition(const glm::vec3& pos) { position = pos; }
void Entity::SetRotation(const glm::vec3& rot) { rotation = rot; }
void Entity::SetScale(const glm::vec3& s) { scale = s; }

glm::mat4 Entity::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}

std::shared_ptr<Mesh> Entity::GetMesh() const {
    return mesh;
}
