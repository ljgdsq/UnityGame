#pragma once
#include "glm/glm.hpp"

struct ThumbnailConfig
{
    int width = 128;
    int height = 128;
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    glm::vec3 rotationX = glm::vec3(45.0f, 0.0f, 0.0f); // 绕X轴旋转角度
    glm::vec3 rotationY = glm::vec3(0.0f, 30.0f, 0.0f); // 绕Y轴旋转角度
    glm::vec4 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
};
