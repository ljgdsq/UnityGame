#pragma once
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"

namespace framework
{
    template <typename AssetType>
    std::shared_ptr<AssetType> AssetReference<AssetType>::LoadAsset() const
    {
        if (m_assetId.empty())
        {
            return nullptr;
        }

        try
        {
            // 从AssetManager加载资源
            auto asset = AssetManager::GetInstance().LoadAsset(m_assetId);
            if (asset)
            {
                // 尝试转换为目标类型
                auto typedAsset = std::dynamic_pointer_cast<AssetType>(asset);
                if (typedAsset)
                {
                    return typedAsset;
                }
                else
                {
                    Logger::Error("Asset {} exists but is not of the expected type", m_assetId);
                }
            }
            else
            {
                Logger::Warn("Failed to load asset: {}", m_assetId);
            }
        }
        catch (const std::exception &e)
        {
            Logger::Error("Exception while loading asset {}: {}", m_assetId, e.what());
        }

        return nullptr;
    }

    template <typename AssetType>
    void AssetReference<AssetType>::LoadAssetAsync(const std::string &assetId,
                                                   std::function<void(std::shared_ptr<Asset>)> callback) const
    {
        if (assetId.empty())
        {
            if (callback)
            {
                callback(nullptr);
            }
            return;
        }

        // 使用AssetManager的异步加载功能
        AssetManager::GetInstance().LoadAssetAsync(assetId, callback);
    }

    template <typename AssetType>
    std::shared_ptr<AssetType> AssetReference<AssetType>::GetDefaultAsset() const
    {
        // 从AssetManager获取默认资源
        return AssetManager::GetInstance().GetDefaultAsset<AssetType>();
    }

    // 针对特定类型的特化实现
    template <>
    inline std::shared_ptr<class MeshAsset> AssetReference<class MeshAsset>::GetDefaultAsset() const
    {
        return AssetManager::GetInstance().GetDefaultMeshAsset();
    }

    template <>
    inline std::shared_ptr<class TextureAsset> AssetReference<class TextureAsset>::GetDefaultAsset() const
    {
        return AssetManager::GetInstance().GetDefaultTextureAsset();
    }

    template <>
    inline std::shared_ptr<class MaterialAsset> AssetReference<class MaterialAsset>::GetDefaultAsset() const
    {
        return AssetManager::GetInstance().GetDefaultMaterialAsset();
    }

} // namespace framework
