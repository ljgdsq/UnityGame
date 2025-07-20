#define GLM_ENABLE_EXPERIMENTAL
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "Transform.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "Framework/Log/Logger.h"

rapidjson::Value framework::Transform::Serialize(rapidjson::Document::AllocatorType &allocator) const
{
    rapidjson::Value value(rapidjson::kObjectType);

    // 创建position数组
    rapidjson::Value posArray(rapidjson::kArrayType);
    posArray.PushBack(position.x, allocator);
    posArray.PushBack(position.y, allocator);
    posArray.PushBack(position.z, allocator);
    value.AddMember("position", posArray, allocator);

    // 创建rotation数组
    rapidjson::Value rotArray(rapidjson::kArrayType);
    rotArray.PushBack(rotation.x, allocator);
    rotArray.PushBack(rotation.y, allocator);
    rotArray.PushBack(rotation.z, allocator);
    rotArray.PushBack(rotation.w, allocator);
    value.AddMember("rotation", rotArray, allocator);

    // 创建scale数组
    rapidjson::Value scaleArray(rapidjson::kArrayType);
    scaleArray.PushBack(scale.x, allocator);
    scaleArray.PushBack(scale.y, allocator);
    scaleArray.PushBack(scale.z, allocator);
    value.AddMember("scale", scaleArray, allocator);

    return value;
}

void framework::Transform::Deserialize(const rapidjson::Value& jsonValue)
{
    if (!jsonValue.IsObject()) {
        Logger::Error("Transform::Deserialize: Invalid JSON object");
        return;
    }

    // 反序列化position
    if (jsonValue.HasMember("position") && jsonValue["position"].IsArray()) {
        const auto& posArray = jsonValue["position"];
        if (posArray.Size() >= 3) {
            position.x = static_cast<float>(posArray[0].GetDouble());
            position.y = static_cast<float>(posArray[1].GetDouble());
            position.z = static_cast<float>(posArray[2].GetDouble());
        }
    }

    // 反序列化rotation
    if (jsonValue.HasMember("rotation") && jsonValue["rotation"].IsArray()) {
        const auto& rotArray = jsonValue["rotation"];
        if (rotArray.Size() >= 4) {
            rotation.x = static_cast<float>(rotArray[0].GetDouble());
            rotation.y = static_cast<float>(rotArray[1].GetDouble());
            rotation.z = static_cast<float>(rotArray[2].GetDouble());
            rotation.w = static_cast<float>(rotArray[3].GetDouble());
        }
    }

    // 反序列化scale
    if (jsonValue.HasMember("scale") && jsonValue["scale"].IsArray()) {
        const auto& scaleArray = jsonValue["scale"];
        if (scaleArray.Size() >= 3) {
            scale.x = static_cast<float>(scaleArray[0].GetDouble());
            scale.y = static_cast<float>(scaleArray[1].GetDouble());
            scale.z = static_cast<float>(scaleArray[2].GetDouble());
        }
    }
}

void framework::Transform::SetRotation(const glm::vec3 &eulerAngles)
{
    // 将欧拉角转换为四元数
    glm::quat quat = glm::quat(glm::radians(eulerAngles));
    rotation = quat;
}

void framework::Transform::Rotate(const glm::vec3 &axis, float angle)
{
    // 创建旋转四元数
    glm::quat deltaRotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

    // 应用旋转
    rotation = deltaRotation * rotation;

    // 确保四元数规范化
    rotation = glm::normalize(rotation);
}

glm::mat4 framework::Transform::GetModelMatrix() const
{
    // 创建变换矩阵
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // 应用平移
    modelMatrix = glm::translate(modelMatrix, position);

    // 应用旋转
    modelMatrix = modelMatrix * glm::toMat4(rotation);

    // 应用缩放
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}

glm::vec3 framework::Transform::GetForward() const
{
    // 在OpenGL中，默认前向是-Z方向
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    return glm::normalize(rotation * forward);
}

glm::vec3 framework::Transform::GetRight() const
{
    // 右向是+X方向
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    return glm::normalize(rotation * right);
}

glm::vec3 framework::Transform::GetUp() const
{
    // 上向是+Y方向
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::normalize(rotation * up);
}
