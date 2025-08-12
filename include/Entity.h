#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Rendering/Mesh.h"


class Entity {
public:
    Entity(std::shared_ptr<Mesh> mesh);

    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);  // or use quaternion
    void SetScale(const glm::vec3& scale);

    glm::mat4 GetModelMatrix() const;

    std::shared_ptr<Mesh> GetMesh() const;

private:
    std::shared_ptr<Mesh> mesh;
    glm::vec3 position;
    glm::vec3 rotation;  // e.g. Euler angles in degrees
    glm::vec3 scale;
};