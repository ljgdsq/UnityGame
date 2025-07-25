#include "Framework/Graphic/RenderComponent.h"
#include "Framework/Graphic/Material.h"
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
            jsonValue.AddMember("material", rapidjson::Value(m_material->GetName().c_str(), allocator), allocator);
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
        if (jsonValue.HasMember("material") && jsonValue["material"].IsString())
        {
            std::string materialName = jsonValue["material"].GetString();
            // 注意: 这里需要从AssetManager加载Material而不是MaterialAsset
            // 暂时设为nullptr，需要实现Material的加载逻辑
            m_material = nullptr;
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
