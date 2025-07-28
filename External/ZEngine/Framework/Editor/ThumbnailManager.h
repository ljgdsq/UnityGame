#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Asset/Icons.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Editor/ThumbnailConfig.h"

namespace framework{
    class MeshAsset;
}

namespace editor
{
    using namespace framework;

    class ThumbnailManager
    {
        SINGLETON_CLASS(ThumbnailManager);
        public:
        std::shared_ptr<TextureAsset> GetThumbnail(AssetType type);
        std::shared_ptr<TextureAsset> GetAssetThumbnail(std::shared_ptr<Asset> asset);
        std::shared_ptr<TextureAsset> GenerateMeshThumbnail(std::shared_ptr<MeshAsset> mesh,
                                                            const std::string& cacheKey = "");

        // cache management
        void ClearThumbnailCache();
        void RemoveThumbnailFromCache(const std::string& cacheKey);
        bool HasCachedThumbnail(const std::string& cacheKey) const;
        void SetDefaultThumbnailConfig(const ThumbnailConfig& config);
        private:
        std::unordered_map<std::string, std::shared_ptr<TextureAsset>> m_thumbnailCache;
        std::unique_ptr<ThumbnailConfig> m_defaultConfig;
        std::string GenerateCacheKey(std::shared_ptr<Asset> asset) const;

    };
}
