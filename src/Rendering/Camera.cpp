#include "Rendering/Camera.h"
#include <cstring>

Camera::Camera(glm::vec3 startPos, glm::vec3 upVec, float startYaw, float startPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)),
      speed(30.0f), sensitivity(0.1f), zoom(45.0f)
{
    position = startPos;
    worldUp = upVec;
    yaw = startYaw;
    pitch = startPitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(const char* direction, float deltaTime) {
    float velocity = speed * deltaTime;

    // Use string compare function - quicker than converting from const char* to std::string
    if (strcmp(direction, "FORWARD") == 0)
        position += front * velocity;
    else if (strcmp(direction, "BACKWARD") == 0)
        position -= front * velocity;
    else if (strcmp(direction, "LEFT") == 0)
        position -= right * velocity;
    else if (strcmp(direction, "RIGHT") == 0)
        position += right * velocity;
    else if (strcmp(direction, "UPWARD") == 0)
        position += up * velocity;
    else if (strcmp(direction, "DOWNWARD") == 0)
        position -= up * velocity;
}


void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    zoom -= yoffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::updateCameraVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);

    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
