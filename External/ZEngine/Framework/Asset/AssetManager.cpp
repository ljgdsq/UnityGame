#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"


namespace framework
{
    void AssetManager::Initialize()
    {
        // Initialize asset manager
        Logger::Log("AssetManager initialized");
        RegisterLoader<UnknownAssetLoader>();
    }

    void AssetManager::Shutdown()
    {
        UnloadAllAssets();
        Logger::Log("AssetManager shutdown");
    }

    void AssetManager::RegisterLoader(std::shared_ptr<AssetLoader> loader)
    {
        if (!loader)
        {
            Logger::Error("Attempted to register a null asset loader");
            return;
        }

        if (std::find(m_loaders.begin(), m_loaders.end(), loader) != m_loaders.end())
        {
            Logger::Warn("Asset loader {} is already registered", loader->GetName());
            return;
        }

        m_loaders.push_back(loader);
        Logger::Log("Registered asset loader: {}", loader->GetName());
    }


    void AssetManager::UnregisterLoader(std::shared_ptr<AssetLoader> loader)
    {
        auto it = std::remove(m_loaders.begin(), m_loaders.end(), loader);
        if (it != m_loaders.end())
        {
            m_loaders.erase(it, m_loaders.end());
            Logger::Log("Unregistered asset loader: {}", loader->GetName());
        }
    }

    std::shared_ptr<Asset> AssetManager::LoadAsset(const std::string &assetPath, AssetType type)
    {
        auto assetName = FileUtil::ExtractFileName(assetPath);
        if (HasAsset(assetPath))
        {
            return GetAsset(assetPath);
        }

        for (const auto &loader : m_loaders)
        {
            if (loader->GetAssetType() == type && loader->CanLoadAsset(assetPath))
            {
                auto asset = loader->LoadAsset(assetPath);
                if (asset)
                {
                    Logger::Log("Loaded asset: {} with type {}", assetPath, loader->GetName());
                    return asset;
                }
                else
                {
                    Logger::Error("Failed to load asset: {}", assetPath);
                }
            }
        }

        if (m_unknownAssetLoader)
        {
            // 如果没有找到合适的加载器，使用未知资源加载器
            auto asset = m_unknownAssetLoader->LoadAsset(assetPath);
            if (asset)
            {
                m_assets[assetPath] = asset;
                Logger::Log("Loaded unknown asset: {}", assetPath);
                return asset;
            }
        }

        Logger::Error("No suitable loader found for asset: {}", assetPath);
        return nullptr;
    }

    std::shared_ptr<Asset> AssetManager::GetAsset(const std::string &assetName)
    {
        auto it = m_assets.find(assetName);
        if (it != m_assets.end())
        {
            return it->second;
        }
        return nullptr;
    }

    bool AssetManager::HasAsset(const std::string &assetName) const
    {
        return m_assets.find(assetName) != m_assets.end();
    }

    void AssetManager::UnloadAsset(const std::string &assetName, bool force)
    {
        auto asset = GetAsset(assetName);
        if (asset)
        {
            if (asset->GetRefCount() == 1 || force)
            {
                m_assets.erase(assetName);
            }
            asset->Release(force);
            Logger::Log("Unloaded asset: {}", assetName);
        }
        else
        {
            Logger::Warn("Attempted to unload non-existent asset: {}", assetName);
        }
    }

    void AssetManager::UnloadAllAssets()
    {
        for (auto &pair : m_assets)
        {
            pair.second->Release(true);
        }
        m_assets.clear();
        Logger::Log("Unloaded all assets");
    }

    long AssetManager::GetTotalAssetsSize() const
    {
        long totalSize = 0;
        for (const auto &pair : m_assets)
        {
            totalSize += pair.second->GetSize();
        }
        return totalSize;
    }

    long AssetManager::GetTotalAssetsCount() const
    {
        return m_assets.size();
    }

    std::vector<std::string> AssetManager::GetAllAssetNames() const
    {
        std::vector<std::string> assetNames;
        for (const auto &pair : m_assets)
        {
            assetNames.push_back(pair.first);
        }
        return assetNames;
    }

} // namespace framework
