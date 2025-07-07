#pragma once
#include <string>
#include <memory>
#include <mutex>
#include "Framework/Asset/Asset.h"

namespace framework
{

    class AssetManager
    {
    public:
        AssetManager() = default;
        ~AssetManager();

        AssetManager *GetInstance()
        {
            if (!s_instance)
            {
                s_instance = new AssetManager();
            }
            return s_instance;
        }

    public:
        void RegisterLoader(std::shared_ptr<AssetLoader> loader);
        void UnregisterLoader(std::shared_ptr<AssetLoader> loader);

        template <class T>
        std::shared_ptr<T> LoadAsset(const std::string &assetPath);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath, AssetType type);

        std::shared_ptr<Asset> GetAsset(const std::string &assetName);
        bool HasAsset(const std::string &assetName) const;
        void UnloadAsset(const std::string &assetName);
        void UnloadAllAssets();

        void PreloadAssets(const std::vector<std::string> &assetPaths);

        long GetTotalAssetsSize() const;
        long GetTotalAssetsCount() const;

        std::vector<std::string> GetAllAssetNames() const;

        // void EnableHotReload(bool enable);

        void CheckForChanges();

    private:
        AssetLoader *FindLoader(const std::string &assetPath) const;
        AssetLoader *FindLoaderByType(AssetType type) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets; // 存储已加载的资源
        std::vector<std::shared_ptr<AssetLoader>> m_loaders;              // 资源加载器列表

        // 线程安全
        mutable std::mutex m_assetMutex;
        mutable std::mutex m_loaderMutex;
        std::unordered_map<std::string, time_t> m_fileTimestamps;

    private:
        static AssetManager *s_instance; // 单例实例
    }

}