#include "Framework/Asset/SceneAsset.h"
namespace framework
{
    SceneAsset::SceneAsset(const std::string &name)
        : Asset(name, AssetType::Scene)
    {
        // Constructor implementation
    }

    long SceneAsset::GetSize() const
    {
        // Return the size of the scene asset, if applicable
        return 0; // Placeholder implementation
    }


    void SceneAsset::GenerateThumbnail()
    {
        // Generate a thumbnail for the scene asset
    }

    rapidjson::Value SceneAsset::Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const {
        rapidjson::Value jsonValue = Asset::Serialize(allocator);
        if (m_Scene)
        {
            jsonValue.AddMember("scene", m_Scene->Serialize(allocator), allocator);
        }
        return jsonValue;
    }

    void SceneAsset::Deserialize(const rapidjson::Value &json) {
        Asset::Deserialize(json);
        if (m_Scene == nullptr) {
            m_Scene = std::make_shared<Scene>();
        }
        if(!json.HasMember("scene")) {
            LOG_ERROR("SceneAsset::Deserialize: Missing 'scene' member in JSON");
            return;
        }

        if (!json["scene"].IsObject()) {
            LOG_ERROR("SceneAsset::Deserialize: 'scene' member is not an object");
            return;
        }

        m_Scene->Deserialize(json["scene"]);
    }

    // Additional methods for SceneAsset can be added here
} // namespace framework
