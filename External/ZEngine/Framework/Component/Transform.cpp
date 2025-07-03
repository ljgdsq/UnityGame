#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "Transform.h"
#include "glm/gtc/type_ptr.hpp"

rapidjson::Value framework::Transform::Serialize() const
{
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
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
