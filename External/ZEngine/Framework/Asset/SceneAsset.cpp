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
        return Asset::Serialize(allocator);
    }

    void SceneAsset::Deserialize(const rapidjson::Value &json) {
        Asset::Deserialize(json);
    }

    // Additional methods for SceneAsset can be added here
} // namespace framework
