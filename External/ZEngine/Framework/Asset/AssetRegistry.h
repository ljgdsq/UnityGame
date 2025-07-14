#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <mutex>
#include "Framework/Asset/Asset.h"

namespace framework
{
    enum class LoadPriority
    {
        Low = 0,
        Normal = 1,
        High = 2,
        Critical = 3
    };

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

    class AssetRegistry
    {
    public:
        static AssetRegistry &GetInstance();

        // 资源注册
        void RegisterAsset(const AssetInfo &info);
        void UnregisterAsset(const std::string &assetId);
        void UpdateAsset(const AssetInfo &info);

        // 资源查询
        const AssetInfo *GetAssetInfo(const std::string &assetId) const;
        std::vector<AssetInfo> GetAssetsByType(AssetType type) const;
        std::vector<AssetInfo> GetAssetsByDirectory(const std::string &directory) const;
        std::vector<AssetInfo> GetAllAssets() const;

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

        // 热重载支持
        void CheckForFileChanges();
        void SetFileWatcher(bool enable);

        // 序列化
        void SaveRegistry(const std::string &filePath) const;
        void LoadRegistry(const std::string &filePath);

        // 统计信息
        size_t GetAssetCount() const;
        size_t GetAssetCountByType(AssetType type) const;
        size_t GetTotalFileSize() const;

        // 清理
        void Clear();

    private:
        AssetRegistry() = default;
        ~AssetRegistry() = default;

        // 禁用拷贝
        AssetRegistry(const AssetRegistry &) = delete;
        AssetRegistry &operator=(const AssetRegistry &) = delete;

        // 辅助方法
        std::string GenerateAssetId(const std::string &filePath) const;
        AssetType DeduceAssetType(const std::string &filePath) const;
        bool IsAssetFile(const std::string &filePath) const;
        void ProcessFile(const std::filesystem::path &filePath);

        // 数据成员
        std::unordered_map<std::string, AssetInfo> m_assets;
        std::vector<std::string> m_resourceDirectories;
        mutable std::mutex m_mutex;
        bool m_fileWatcherEnabled = false;

        // 支持的文件扩展名
        static const std::unordered_map<std::string, AssetType> s_supportedExtensions;
    };

    // 全局便利函数
    AssetRegistry &GetAssetRegistry();

} // namespace framework
