#pragma once

#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Common/Define.h"

namespace framework
{

    /**
     * @brief 资源管理器类，用于管理游戏中的所有资源
     * * 负责资源的加载、卸载、引用计数和依赖关系管理。
     * * 支持多种资源类型的加载和管理。
     * todo: 支持资源的异步加载和缓存机制。
     */
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

        void UnregisterLoader(std::shared_ptr<AssetLoader> loader);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath, AssetType type);

        // std::shared_ptr<Asset> LoadAsset(const std::string &assetId); // 通过ID加载

        // 同步加载
        template <class T>
        std::shared_ptr<T> LoadAsset(const std::string &assetPath);

        // Asset管理
        std::shared_ptr<Asset> GetAsset(const std::string &assetName);

        // std::shared_ptr<Asset> GetAssetById(const std::string &assetId);

        bool HasAsset(const std::string &assetName) const;

        void UnloadAsset(const std::string &assetName, bool force = false);

        // void UnloadAssetById(const std::string &assetId);

        void UnloadAllAssets();

        // 统计信息
        long GetTotalAssetsSize() const;

        long GetTotalAssetsCount() const;

        std::vector<std::string> GetAllAssetNames() const;

        // std::vector<std::string> GetAllAssetIds() const;

        //        // 内存管理
        //        void SetCacheSize(size_t maxSize) { m_maxCacheSize = maxSize; }
        //        void GarbageCollect(); // 清理未使用的资源

    private:
        std::vector<std::shared_ptr<AssetLoader>> m_loaders;              // 加载器列表
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets; // 资源映射，使用资源名称作为键
    };
    // 注册默认加载器
    void RegisterDefaultLoader();

}
