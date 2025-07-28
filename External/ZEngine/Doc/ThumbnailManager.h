// Framework/Editor/ThumbnailManager.h
#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Framework/Asset/Asset.h"
#include "Framework/Asset/TextureAsset.h"

namespace framework
{
    class Mesh;
    class Material;
    struct ThumbnailConfig;
}

namespace editor
{
    class ThumbnailManager
    {
        SINGLETON_CLASS(ThumbnailManager);

    public:
        // 现有方法：获取类型图标
        std::shared_ptr<framework::TextureAsset> GetThumbnail(framework::AssetType type);

        // 新增方法：获取资产缩略图
        std::shared_ptr<framework::TextureAsset> GetAssetThumbnail(std::shared_ptr<framework::Asset> asset);

        // 具体资产类型的缩略图生成
        std::shared_ptr<framework::TextureAsset> GenerateMeshThumbnail(std::shared_ptr<framework::Mesh> mesh,
                                                                       const std::string& cacheKey = "");
        std::shared_ptr<framework::TextureAsset> GenerateMaterialThumbnail(std::shared_ptr<framework::Material> material,
                                                                           const std::string& cacheKey = "");

        // 缓存管理
        void ClearThumbnailCache();
        void RemoveThumbnailFromCache(const std::string& cacheKey);
        bool HasCachedThumbnail(const std::string& cacheKey) const;

        // 配置
        void SetDefaultThumbnailConfig(const framework::ThumbnailConfig& config);

    private:
        // 缩略图缓存
        std::unordered_map<std::string, std::shared_ptr<framework::TextureAsset>> m_thumbnailCache;

        // 默认配置
        std::unique_ptr<framework::ThumbnailConfig> m_defaultConfig;

        // 生成缓存键
        std::string GenerateCacheKey(std::shared_ptr<framework::Asset> asset) const;

        // 将渲染结果转换为TextureAsset
        std::shared_ptr<framework::TextureAsset> CreateTextureAssetFromRender(void* textureId,
                                                                              const std::string& name) const;
    };

} // namespace editor
