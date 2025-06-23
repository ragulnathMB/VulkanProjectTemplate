#include "Camera.hpp"

Camera::Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
    : position(0.0f, 0.0f, 3.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    fov(fovDegrees),
    aspect(aspectRatio),
    nearClip(nearPlane),
    farClip(farPlane) {
}

// Set camera position
void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

// Set point camera looks at
void Camera::setTarget(const glm::vec3& tgt) {
    target = tgt;
}

// Update aspect ratio (e.g. window resize)
void Camera::setAspectRatio(float aspectRatio) {
    aspect = aspectRatio;
}

// Get view matrix (from position and target)
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

// Get perspective projection matrix
glm::mat4 Camera::getProjectionMatrix() const {
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    proj[1][1] *= -1; // Flip Y for Vulkan
    return proj;
}
