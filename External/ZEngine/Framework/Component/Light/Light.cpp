#include "Framework/Component/Light/Light.h"
#include "Framework/Manager/LightManager.h"
#include "glm/gtc/type_ptr.hpp"

namespace framework
{
    void Light::OnEnable()
    {
        LightManager::RegisterLight(this);
    }

    void Light::OnDisable()
    {
        LightManager::UnregisterLight(this);
    }

    rapidjson::Value Light::Serialize() const
    {
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Value typeValue;
        typeValue.SetString(GetTypeName(), doc.GetAllocator());
        doc.AddMember("type", typeValue, doc.GetAllocator());

        doc.AddMember("lightType", static_cast<int>(type), doc.GetAllocator());

        // 正确地序列化颜色数组
        rapidjson::Value colorArray(rapidjson::kArrayType);
        colorArray.PushBack(static_cast<double>(color.r), doc.GetAllocator());
        colorArray.PushBack(static_cast<double>(color.g), doc.GetAllocator());
        colorArray.PushBack(static_cast<double>(color.b), doc.GetAllocator());
        doc.AddMember("color", colorArray, doc.GetAllocator());

        doc.AddMember("intensity", static_cast<double>(intensity), doc.GetAllocator());

        return doc;
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