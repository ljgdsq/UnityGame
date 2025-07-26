#pragma once

#include "Framework/Log/Logger.h"
#include "Framework/Common/Define.h"
#include "Framework/Util/FileUtil.hpp"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Asset/Asset.h"
#include "Framework/Asset/UnknownAsset.h"
#include "Framework/Asset/UnknownAssetLoader.h"
namespace framework
{

    class AssetManager
    {

        SINGLETON_CLASS(AssetManager);

    public:
        // 初始化和清理
        void Initialize();

        void Shutdown();

    public:
        // 加载器管理
        void RegisterLoader(std::shared_ptr<AssetLoader> loader);

        template <class T>
        void RegisterLoader()
        {
            {
                auto loader = std::make_shared<T>();
                if (loader)
                {
                    RegisterLoader(loader);
                }
                else
                {
                    Logger::Error("Failed to create asset loader of type: {}", typeid(T).name());
                }
            }
        }

        void UnregisterLoader(std::shared_ptr<AssetLoader> loader);

        // 同步加载
        template <class T>
        std::shared_ptr<T> LoadAsset(const std::string &assetPath)
        {
            auto assetName = FileUtil::ExtractFileName(assetPath);
            if (HasAsset(assetName))
            {
                return std::dynamic_pointer_cast<T>(GetAsset(assetName));
            }

            for (const auto &loader : m_loaders)
            {
                if (loader->CanLoadAsset(assetPath))
                {
                    auto asset = loader->LoadAsset(assetPath);
                    if (asset)
                    {
                        m_assets[assetName] = asset;
                        Logger::Log("Loaded asset: {} with type {}", assetPath, loader->GetName());
                        return std::dynamic_pointer_cast<T>(asset);
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
                    m_assets[assetName] = asset;
                    Logger::Log("Loaded unknown asset: {}", assetPath);
                    return std::dynamic_pointer_cast<T>(asset);
                }
            }
            Logger::Error("No suitable loader found for asset: {}", assetPath);
            return nullptr;
        }

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath, AssetType type);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetId); // 通过ID加载

        // Asset管理
        std::shared_ptr<Asset> GetAsset(const std::string &assetName);

        template <class T>
        std::shared_ptr<T> GetAsset(const std::string &assetName)
        {
            auto asset = GetAsset(assetName);
            if (asset)
            {
                return std::dynamic_pointer_cast<T>(asset);
            }
            return nullptr;
        }

        std::shared_ptr<Asset> GetAssetById(const std::string &assetId);

        bool HasAsset(const std::string &assetName) const;

        void UnloadAsset(const std::string &assetName, bool force = false);

        void UnloadAllAssets();

        // 统计信息
        long GetTotalAssetsSize() const;

        long GetTotalAssetsCount() const;

        std::vector<std::string> GetAllAssetNames() const;

        //        // 内存管理
        //        void SetCacheSize(size_t maxSize) { m_maxCacheSize = maxSize; }
        //        void GarbageCollect(); // 清理未使用的资源

    private:
    private:
        std::vector<std::shared_ptr<AssetLoader>> m_loaders; // 加载器列表
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;

        std::shared_ptr<AssetLoader> m_unknownAssetLoader; // 用于加载未知类型的资源
    };

    void RegisterDefaultLoader();

}
