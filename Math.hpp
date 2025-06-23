#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

namespace Math {

    constexpr float PI = 3.14159265359f;
    constexpr float EPSILON = 1e-6f;

    // Converts degrees to radians
    inline float toRadians(float degrees) {
        return degrees * PI / 180.0f;
    }

    // Converts radians to degrees
    inline float toDegrees(float radians) {
        return radians * 180.0f / PI;
    }

    // Clamps a value between min and max
    template<typename T>
    inline T clamp(T value, T minVal, T maxVal) {
        return (value < minVal) ? minVal : ((value > maxVal) ? maxVal : value);
    }

    // Linear interpolation
    template<typename T>
    inline T lerp(const T& a, const T& b, float t) {
        return a + t * (b - a);
    }

    // Returns normalized vector, safe version
    inline glm::vec3 safeNormalize(const glm::vec3& v) {
        float lenSq = glm::length2(v);
        if (lenSq > EPSILON)
            return glm::normalize(v);
        return glm::vec3(0.0f);
    }

    // Create a model matrix from position, rotation, and scale
    inline glm::mat4 createModelMatrix(const glm::vec3& position,
        const glm::vec3& rotationEulerDegrees,
        const glm::vec3& scale) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);

        glm::vec3 rotRad = glm::radians(rotationEulerDegrees);
        model = glm::rotate(model, rotRad.x, glm::vec3(1, 0, 0));
        model = glm::rotate(model, rotRad.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, rotRad.z, glm::vec3(0, 0, 1));

        model = glm::scale(model, scale);
        return model;
    }

    // Get direction vector from pitch and yaw angles (in degrees)
    inline glm::vec3 getDirectionFromAngles(float pitch, float yaw) {
        float radPitch = toRadians(pitch);
        float radYaw = toRadians(yaw);

        return glm::normalize(glm::vec3(
            cos(radPitch) * sin(radYaw),
            sin(radPitch),
            cos(radPitch) * cos(radYaw)
        ));
    }
}
