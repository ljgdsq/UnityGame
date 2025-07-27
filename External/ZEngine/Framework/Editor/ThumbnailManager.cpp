#include "Framework/Editor/ThumbnailManager.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/TextureAsset.h"
namespace editor
{
    using namespace framework;
    std::shared_ptr<TextureAsset> ThumbnailManager::GetThumbnail(framework::AssetType type)
    {
        std::string iconPath = Icons::GetIconPath(type);
        if (iconPath.empty())
        {
            LOG_WARN("No icon path found for asset type: {}", static_cast<int>(type));
            return nullptr; // 如果没有找到对应的图标路径，返回空指针
        }

        // 使用 AssetManager 加载图标资源
        auto textureAsset = AssetManager::GetInstance().LoadAsset<TextureAsset>(iconPath);
        if (!textureAsset)
        {
            LOG_ERROR("Failed to load thumbnail for asset type: {}", static_cast<int>(type));
            return nullptr;
        }

        return textureAsset;
    }
} // namespace editor
