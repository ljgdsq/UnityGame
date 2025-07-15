#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

namespace framework
{
    enum class AssetType
    {
        Unknown = 0, // 未知类型
        Texture,
        Shader,
        Mesh,
        Material,
        Audio,
        Animation,
        Font,
        Count // 用于计数
    };

    enum class LoadState
    {
        NotLoaded = 0,
        Loading,
        Loaded,
        Failed
    };

    class Asset
    {
    public:
        Asset(const std::string &name, AssetType type);
        Asset(const std::string &name, AssetType type, const std::string &assetId);
        virtual ~Asset() = default;

        // 基本信息
        AssetType GetType() const { return m_type; }
        const std::string &GetName() const { return m_name; }
        const std::string &GetAssetId() const { return m_assetId; }
        const std::string &GetFilePath() const { return m_filePath; }

        // 设置文件路径
        void SetFilePath(const std::string &filePath) { m_filePath = filePath; }

        // 加载状态管理
        bool IsLoaded() const { return m_loadState == LoadState::Loaded; }
        bool IsLoading() const { return m_loadState == LoadState::Loading; }
        bool HasFailed() const { return m_loadState == LoadState::Failed; }
        LoadState GetLoadState() const { return m_loadState; }

        // 内部使用的状态设置
        void SetLoadState(LoadState state) { m_loadState = state; }

        // 引用计数管理
        void AddRef() { ++m_refCount; }
        void Release();
        int GetRefCount() const { return m_refCount; }

        // 资源操作
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual long GetSize() const = 0;

        // 序列化支持
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;
        virtual void Deserialize(const rapidjson::Value &json);

        // 缩略图支持（用于Editor）
        virtual void *GetThumbnailTextureId() const { return m_thumbnailTextureId; }
        virtual void GenerateThumbnail() {}
        virtual bool HasThumbnail() const { return m_thumbnailTextureId != nullptr; }

        // 依赖关系管理
        const std::vector<std::string> &GetDependencies() const { return m_dependencies; }
        void AddDependency(const std::string &assetId);
        void RemoveDependency(const std::string &assetId);
        void ClearDependencies() { m_dependencies.clear(); }

        // 时间戳支持
        void SetLastModified(std::time_t time) { m_lastModified = time; }
        std::time_t GetLastModified() const { return m_lastModified; }

    protected:
        // 生成唯一的Asset ID
        std::string GenerateAssetId() const;

        // 设置缩略图
        void SetThumbnailTextureId(void *textureId) { m_thumbnailTextureId = textureId; }

    private:
        // 基本信息
        std::string m_name;     // 资源名称
        std::string m_assetId;  // 唯一标识符
        std::string m_filePath; // 文件路径
        AssetType m_type;       // 资源类型

        // 状态管理
        LoadState m_loadState = LoadState::NotLoaded;
        int m_refCount = 0;             // 引用计数
        std::time_t m_lastModified = 0; // 最后修改时间

        // 缩略图
        void *m_thumbnailTextureId = nullptr;

        // 依赖关系
        std::vector<std::string> m_dependencies;
    };

    // 辅助函数
    std::string AssetTypeToString(AssetType type);
    AssetType StringToAssetType(const std::string &typeStr);
}
