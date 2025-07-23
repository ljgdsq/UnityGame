#pragma once

#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Common/Define.h"

namespace framework {
    class AssetManager {

    SINGLETON_CLASS(AssetManager);
    public:


        // 初始化和清理
        void Initialize();

        void Shutdown();

    public:
        // 加载器管理
        void RegisterLoader(std::shared_ptr<AssetLoader> loader);

        void UnregisterLoader(std::shared_ptr<AssetLoader> loader);

        // 同步加载
        template<class T>
        std::shared_ptr<T> LoadAsset(const std::string &assetPath);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath, AssetType type);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetId); // 通过ID加载


        // Asset管理
        std::shared_ptr<Asset> GetAsset(const std::string &assetName);

        std::shared_ptr<Asset> GetAssetById(const std::string &assetId);

        bool HasAsset(const std::string &assetName) const;

        void UnloadAsset(const std::string &assetName,bool force=false);


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
        std::vector<std::shared_ptr<AssetLoader>> m_loaders;                  // 加载器列表
        std::unordered_map<std::string,std::shared_ptr<Asset>> m_assets;
    };


    void RegisterDefaultLoader();

}

