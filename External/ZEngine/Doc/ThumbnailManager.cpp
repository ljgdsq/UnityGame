// Framework/Editor/ThumbnailManager.cpp
#include "Framework/Editor/ThumbnailManager.h"
#include "Framework/Graphic/ThumbnailRenderer.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Core/Mesh.h"
#include "Framework/Log/Logger.h"

namespace editor
{
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

        switch (asset->GetAssetType())
        {
        case framework::AssetType::Mesh:
        {
            auto meshAsset = std::dynamic_pointer_cast<framework::MeshAsset>(asset);
            if (meshAsset && meshAsset->GetMesh())
            {
                thumbnail = GenerateMeshThumbnail(meshAsset->GetMesh(), cacheKey);
            }
            break;
        }
        case framework::AssetType::Material:
        {
            auto materialAsset = std::dynamic_pointer_cast<framework::MaterialAsset>(asset);
            if (materialAsset)
            {
                // thumbnail = GenerateMaterialThumbnail(materialAsset->GetMaterial(), cacheKey);
            }
            break;
        }
        default:
            // 使用类型图标作为回退
            thumbnail = GetThumbnail(asset->GetAssetType());
            break;
        }

        // 缓存结果
        if (thumbnail)
        {
            m_thumbnailCache[cacheKey] = thumbnail;
        }

        return thumbnail ? thumbnail : GetThumbnail(asset->GetAssetType());
    }

    std::shared_ptr<framework::TextureAsset> ThumbnailManager::GenerateMeshThumbnail(std::shared_ptr<framework::Mesh> mesh,
                                                                                     const std::string &cacheKey)
    {
        if (!mesh)
        {
            LOG_ERROR("ThumbnailManager: Cannot generate thumbnail for null mesh");
            return nullptr;
        }

        // 使用默认配置或自定义配置
        framework::ThumbnailConfig config = m_defaultConfig ? *m_defaultConfig : framework::ThumbnailConfig{};

        // 使用ThumbnailRenderer进行渲染
        void *textureId = framework::ThumbnailRenderer::GetInstance().RenderMeshThumbnail(mesh, config);

        if (!textureId)
        {
            LOG_ERROR("ThumbnailManager: Failed to render mesh thumbnail");
            return nullptr;
        }

        // 转换为TextureAsset
        std::string thumbnailName = cacheKey.empty() ? "mesh_thumbnail" : cacheKey;
        return CreateTextureAssetFromRender(textureId, thumbnailName);
    }

    std::string ThumbnailManager::GenerateCacheKey(std::shared_ptr<framework::Asset> asset) const
    {
        if (!asset)
        {
            return "";
        }

        // 使用资产ID + 修改时间戳作为缓存键
        return asset->GetAssetId() + "_" + std::to_string(asset->GetLastModifiedTime());
    }

    bool ThumbnailManager::HasCachedThumbnail(const std::string &cacheKey) const
    {
        return m_thumbnailCache.find(cacheKey) != m_thumbnailCache.end();
    }

    void ThumbnailManager::ClearThumbnailCache()
    {
        m_thumbnailCache.clear();
        LOG_INFO("ThumbnailManager: Thumbnail cache cleared");
    }

    void ThumbnailManager::SetDefaultThumbnailConfig(const framework::ThumbnailConfig &config)
    {
        m_defaultConfig = std::make_unique<framework::ThumbnailConfig>(config);
    }

} // namespace editor
