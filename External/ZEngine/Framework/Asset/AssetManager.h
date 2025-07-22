#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>
#include <filesystem>
#include <ctime>
#include "Framework/Asset/Asset.h"
#include "Framework/Log/Logger.h"
#include "rapidjson/document.h"

namespace framework
{
    class AssetLoader;
    class TextureAsset;
    class MeshAsset;
    class MaterialAsset;

    enum class LoadPriority
    {
        Low = 0,
        Normal = 1,
        High = 2,
        Critical = 3
    };

    // 资源信息结构体（从AssetRegistry迁移）
    struct AssetInfo
    {
        std::string assetId;          // 唯一标识
        std::string displayName;      // 显示名称
        std::string filePath;         // 文件路径
        AssetType type;               // 资源类型
        size_t fileSize = 0;          // 文件大小
        std::time_t lastModified = 0; // 最后修改时间

        // 缩略图信息（延迟生成）
        std::string thumbnailPath;
        void *thumbnailTextureId = nullptr;

        // 加载状态
        LoadState loadState = LoadState::NotLoaded;
        LoadPriority priority = LoadPriority::Normal;

        // 依赖关系
        std::vector<std::string> dependencies;
        std::vector<std::string> dependents;

        // 序列化支持
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;
        void Deserialize(const rapidjson::Value &json);

        // 辅助方法
        std::string GetFileExtension() const;
        std::string GetFileName() const;
        std::string GetDirectory() const;
        bool IsNewer(const AssetInfo &other) const;
    };

    // 异步加载请求结构
    struct LoadRequest
    {
        std::string assetId;
        LoadPriority priority;
        std::function<void(std::shared_ptr<Asset>)> callback;

        // 优先队列比较函数
        bool operator<(const LoadRequest &other) const
        {
            return priority < other.priority; // 高优先级在前
        }
    };

    class AssetManager
    {
    public:
        AssetManager() = default;
        ~AssetManager();

        static AssetManager &GetInstance();

        // 初始化和清理
        void Initialize();
        void Shutdown();

    public:
        // 加载器管理
        void RegisterLoader(std::shared_ptr<AssetLoader> loader);
        void UnregisterLoader(std::shared_ptr<AssetLoader> loader);

        // 同步加载
        template <class T>
        std::shared_ptr<T> LoadAsset(const std::string &assetPath);

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath, AssetType type);
        std::shared_ptr<Asset> LoadAsset(const std::string &assetId); // 通过ID加载

        // 异步加载
        void LoadAssetAsync(const std::string &assetId,
                            std::function<void(std::shared_ptr<Asset>)> callback,
                            LoadPriority priority = LoadPriority::Normal);

        // Asset管理
        std::shared_ptr<Asset> GetAsset(const std::string &assetName);
        std::shared_ptr<Asset> GetAssetById(const std::string &assetId);
        bool HasAsset(const std::string &assetName) const;
        bool HasAssetById(const std::string &assetId) const;
        void UnloadAsset(const std::string &assetName);
        void UnloadAssetById(const std::string &assetId);
        void UnloadAllAssets();

        // 批量操作
        void PreloadAssets(const std::vector<std::string> &assetPaths);
        void PreloadAssets(const std::vector<std::string> &assetIds, LoadPriority priority);

        // 默认资源管理
        template <typename T>
        std::shared_ptr<T> GetDefaultAsset();

        std::shared_ptr<TextureAsset> GetDefaultTextureAsset();
        std::shared_ptr<MeshAsset> GetDefaultMeshAsset();
        std::shared_ptr<MaterialAsset> GetDefaultMaterialAsset();

        void SetDefaultTextureAsset(std::shared_ptr<TextureAsset> asset);
        void SetDefaultMeshAsset(std::shared_ptr<MeshAsset> asset);
        void SetDefaultMaterialAsset(std::shared_ptr<MaterialAsset> asset);

        // 便利方法 - 通过类型创建资源
        std::shared_ptr<TextureAsset> CreateTextureAsset(const std::string &name);
        std::shared_ptr<MeshAsset> CreateMeshAsset(const std::string &name);
        std::shared_ptr<Asset> CreateAsset(const std::string &name, AssetType type);

        // 手动注册已创建的资源
        void RegisterAsset(std::shared_ptr<Asset> asset);

        // 统计信息
        long GetTotalAssetsSize() const;
        long GetTotalAssetsCount() const;
        std::vector<std::string> GetAllAssetNames() const;
        std::vector<std::string> GetAllAssetIds() const;

        // 热重载
        void CheckForChanges();
        void EnableHotReload(bool enable) { m_hotReloadEnabled = enable; }

        // 内存管理
        void SetCacheSize(size_t maxSize) { m_maxCacheSize = maxSize; }
        void GarbageCollect(); // 清理未使用的资源

        // ===== 以下是从AssetRegistry合并的功能 =====

        // 资源信息管理
        void RegisterAssetInfo(const AssetInfo &info);
        void UnregisterAssetInfo(const std::string &assetId);
        void UpdateAssetInfo(const AssetInfo &info);
        const AssetInfo *GetAssetInfo(const std::string &assetId) const;
        std::vector<AssetInfo> GetAssetInfosByType(AssetType type) const;
        std::vector<AssetInfo> GetAssetInfosByDirectory(const std::string &directory) const;
        std::vector<AssetInfo> GetAllAssetInfos() const;

        // 文件系统操作
        void ScanResourceDirectories();
        void ScanDirectory(const std::string &directory, bool recursive = true);
        void AddResourceDirectory(const std::string &directory);
        void RemoveResourceDirectory(const std::string &directory);

        // 资源搜索
        std::vector<AssetInfo> SearchAssets(const std::string &query) const;
        std::vector<AssetInfo> FindAssetsByName(const std::string &name) const;
        std::vector<AssetInfo> FindAssetsByExtension(const std::string &extension) const;

        // 依赖关系管理
        void AddDependency(const std::string &assetId, const std::string &dependencyId);
        void RemoveDependency(const std::string &assetId, const std::string &dependencyId);
        std::vector<std::string> GetDependencies(const std::string &assetId) const;
        std::vector<std::string> GetDependents(const std::string &assetId) const;

        // 文件监控
        void SetFileWatcher(bool enable);

        // 序列化（资源数据库）
        void SaveAssetDatabase(const std::string &filePath) const;
        void LoadAssetDatabase(const std::string &filePath);

        // 扩展统计信息
        size_t GetAssetInfoCount() const;
        size_t GetAssetInfoCountByType(AssetType type) const;
        size_t GetTotalFileSize() const;

    private:
        // 加载相关
        AssetLoader *FindLoader(const std::string &assetPath) const;
        AssetLoader *FindLoaderByType(AssetType type) const;
        std::shared_ptr<Asset> LoadAssetSync(const std::string &assetId);

        // 异步加载线程
        void StartAsyncLoading();
        void StopAsyncLoading();
        void LoadingWorker();

        // 默认资源初始化
        void InitializeDefaultAssets();
        void CreateDefaultTexture();
        void CreateDefaultMesh();
        void CreateDefaultMaterial();

        // 内存管理
        void EvictUnusedAssets();
        bool ShouldEvictAsset(const std::shared_ptr<Asset> &asset) const;

        // ===== 从AssetRegistry合并的私有方法 =====

        // 文件处理辅助方法
        void ProcessFile(const std::filesystem::path &filePath);
        std::string GenerateAssetId(const std::string &filePath) const;
        AssetType DeduceAssetType(const std::string &filePath) const;
        bool IsAssetFile(const std::string &filePath) const;

    private:
        // 资源存储
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;     // 按名称存储
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assetsById; // 按ID存储
        std::vector<std::shared_ptr<AssetLoader>> m_loaders;                  // 加载器列表

        // ===== 从AssetRegistry合并的数据成员 =====

        // 资源信息存储
        std::unordered_map<std::string, AssetInfo> m_assetInfos; // 资源元数据
        std::vector<std::string> m_resourceDirectories;          // 资源目录列表

        // 默认资源
        std::shared_ptr<TextureAsset> m_defaultTexture;
        std::shared_ptr<MeshAsset> m_defaultMesh;
        std::shared_ptr<MaterialAsset> m_defaultMaterial;

        // 异步加载
        std::priority_queue<LoadRequest> m_loadQueue;
        std::vector<std::thread> m_loadingThreads;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCondition;
        bool m_asyncLoadingEnabled = false;

        // 线程安全
        mutable std::mutex m_assetMutex;
        mutable std::mutex m_loaderMutex;
        mutable std::mutex m_assetInfoMutex; // 用于保护资源信息的访问

        // 热重载
        std::unordered_map<std::string, time_t> m_fileTimestamps;
        bool m_hotReloadEnabled = false;
        bool m_fileWatcherEnabled = false;

        // 内存管理
        size_t m_maxCacheSize = 512 * 1024 * 1024; // 512MB
        size_t m_currentCacheSize = 0;

        // 支持的文件扩展名（从AssetRegistry迁移）
        static const std::unordered_map<std::string, AssetType> s_supportedExtensions;

        // 单例
        static AssetManager *s_instance;
    };

    // 全局便利函数
    AssetManager &GetAssetManager();
}

#endif
