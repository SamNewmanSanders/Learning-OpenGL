#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 startPos, glm::vec3 worldUp, float startYaw, float startPitch);

    glm::mat4 getViewMatrix() const;

    void processKeyboard(const char* direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    float getZoom() const;
    glm::vec3 getPosition() const { return position; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    void updateCameraVectors();
};