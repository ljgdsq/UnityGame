#include "Framework/Graphic/RenderComponent.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
namespace framework
{
    rapidjson::Value RenderComponent::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);
        jsonValue.AddMember("type", rapidjson::Value("RenderComponent", allocator), allocator);
        jsonValue.AddMember("visible", m_visible, allocator);
        if (m_material)
        {

            jsonValue.AddMember("material", m_material->Serialize(allocator), allocator);
        }
        jsonValue.AddMember("renderLayer", m_renderLayer, allocator);
        jsonValue.AddMember("sortingOrder", m_sortingOrder, allocator);
        return jsonValue;
    }

    void RenderComponent::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (jsonValue.HasMember("visible") && jsonValue["visible"].IsBool())
        {
            m_visible = jsonValue["visible"].GetBool();
        }
        if (jsonValue.HasMember("material") && jsonValue["material"].IsObject())
        {
            const rapidjson::Value& mtValue = jsonValue["material"];
            if (mtValue.HasMember("filePath"))
            {
                std::string materialPath = mtValue["filePath"].GetString();
                if(materialPath.empty())
                {
                    Logger::Error("Material path is empty in RenderComponent deserialization.");
                    m_material = std::make_shared<MaterialAsset>(mtValue["name"].GetString());
                }else{
                    m_material = AssetManager::GetInstance().LoadAsset<MaterialAsset>(materialPath);
                }

                if (!m_material)
                {
                    Logger::Error("Failed to load material: {}", materialPath);
                }
                m_material = std::make_shared<MaterialAsset>(m_material->GetName());
            }
            m_material->Deserialize(mtValue);
        }
        if (jsonValue.HasMember("renderLayer") && jsonValue["renderLayer"].IsInt())
        {
            m_renderLayer = jsonValue["renderLayer"].GetInt();
        }
        if (jsonValue.HasMember("sortingOrder") && jsonValue["sortingOrder"].IsInt())
        {
            m_sortingOrder = jsonValue["sortingOrder"].GetInt();
        }
    }

}
