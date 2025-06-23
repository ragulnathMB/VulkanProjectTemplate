#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setAspectRatio(float aspectRatio);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getTarget() const { return target; }

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float fov;
    float aspect;
    float nearClip;
    float farClip;
};
