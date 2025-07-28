#include "Framework/Editor/ThumbnailManager.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Editor/ThumbnailGenerator.h"
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

    std::shared_ptr<framework::TextureAsset> ThumbnailManager::GetAssetThumbnail(std::shared_ptr<framework::Asset> asset)
    {
        if (!asset)
        {
            return GetThumbnail(framework::AssetType::Unknown);
        }

        // 生成缓存键
        std::string cacheKey = GenerateCacheKey(asset);

        // 检查缓存
        if (HasCachedThumbnail(cacheKey))
        {
            return m_thumbnailCache[cacheKey];
        }

        // 根据资产类型生成缩略图
        std::shared_ptr<framework::TextureAsset> thumbnail;

        switch (asset->GetType())
        {
        case framework::AssetType::Mesh:
        {
            auto meshAsset = std::dynamic_pointer_cast<framework::MeshAsset>(asset);
            if (meshAsset && meshAsset->GetMesh())
            {
                thumbnail = GenerateMeshThumbnail(meshAsset, cacheKey);
            }
        }
        break;
        default:
            // 使用类型图标作为回退
            thumbnail = GetThumbnail(asset->GetType());
            break;
        }
        // 缓存结果
        if (thumbnail)
        {
            m_thumbnailCache[cacheKey] = thumbnail;
        }

        return thumbnail ? thumbnail : GetThumbnail(asset->GetType());
    }

    std::shared_ptr<framework::TextureAsset> ThumbnailManager::GenerateMeshThumbnail(std::shared_ptr<framework::MeshAsset> mesh, const std::string &cacheKey)
    {
        if (!mesh)
        {
            LOG_ERROR("ThumbnailManager: Cannot generate thumbnail for null mesh");
            return nullptr;
        }

        ThumbnailConfig config;
        if (m_defaultConfig)
        {
            config = *(m_defaultConfig.get());
        }
        else
        {
            config = ThumbnailConfig();
        }

        return ThumbnailGenerator::GetInstance().GenerateThumbnail(mesh, config);
    }

    std::string ThumbnailManager::GenerateCacheKey(std::shared_ptr<framework::Asset> asset) const
    {
        if (!asset)
        {
            return "";
        }

        return asset->GetAssetId() + "_" + std::to_string(asset->GetSize());
    }

    bool ThumbnailManager::HasCachedThumbnail(const std::string &cacheKey) const
    {
        return m_thumbnailCache.find(cacheKey) != m_thumbnailCache.end();
    }

    void ThumbnailManager::ClearThumbnailCache()
    {
        m_thumbnailCache.clear();
        LOG_INFO("Thumbnail cache cleared");
    }

    void ThumbnailManager::SetDefaultThumbnailConfig(const ThumbnailConfig &config)
    {
        m_defaultConfig = std::make_unique<ThumbnailConfig>(config);
    }

    void ThumbnailManager::RemoveThumbnailFromCache(const std::string &cacheKey)
    {
        if (HasCachedThumbnail(cacheKey))
        {
            m_thumbnailCache.erase(cacheKey);
            LOG_INFO("Thumbnail with cache key '{}' removed from cache", cacheKey);
        }
    }

} // namespace editor
