#include "Framework/Component/Light/Light.h"
#include "Framework/Manager/LightManager.h"
#include "glm/gtc/type_ptr.hpp"

namespace framework
{

    void Light::OnEnable()
    {
        Component::OnEnable();
        LightManager::RegisterLight(this);
    }

    void Light::OnDisable()
    {
        Component::OnDisable();
        LightManager::UnregisterLight(this);
    }

    rapidjson::Value Light::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {

        rapidjson::Value typeValue(rapidjson::kObjectType);
        typeValue.AddMember("type", "Light", allocator);
        rapidjson::Value lightTypeValue(rapidjson::kNumberType);
        lightTypeValue.SetInt(static_cast<int>(type));
        typeValue.AddMember("lightType", lightTypeValue, allocator);

        // 正确地序列化颜色数组
        rapidjson::Value colorArray(rapidjson::kArrayType);
        colorArray.PushBack(static_cast<double>(color.r), allocator);
        colorArray.PushBack(static_cast<double>(color.g), allocator);
        colorArray.PushBack(static_cast<double>(color.b), allocator);
        typeValue.AddMember("color", colorArray, allocator);

        typeValue.AddMember("intensity", static_cast<double>(intensity), allocator);

        return typeValue;
    }

    void Light::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (jsonValue.HasMember("lightType") && jsonValue["lightType"].IsInt())
        {
            type = static_cast<LightType>(jsonValue["lightType"].GetInt());
        }

        if (jsonValue.HasMember("color") && jsonValue["color"].IsArray() && jsonValue["color"].Size() == 3)
        {
            const auto &colorArray = jsonValue["color"];
            color = glm::vec3(static_cast<float>(colorArray[0].GetDouble()),
                              static_cast<float>(colorArray[1].GetDouble()),
                              static_cast<float>(colorArray[2].GetDouble()));
        }

        if (jsonValue.HasMember("intensity") && jsonValue["intensity"].IsNumber())
        {
            intensity = static_cast<float>(jsonValue["intensity"].GetDouble());
        }
    }

}
