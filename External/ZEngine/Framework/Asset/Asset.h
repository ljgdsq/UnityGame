#pragma once

#include <string>
#include <vector>
#include <memory>
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
        Video,
        Animation,
        Font,
        Scene,
        Script,
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

        virtual ~Asset() = default;

        // 基本信息
        AssetType GetType() const { return m_type; };

        const std::string &GetName() const { return m_name; };

        const std::string &GetAssetId() const { return m_filePath; }

        const std::string &GetFilePath() const { return m_filePath; }

        void SetFilePath(const std::string &filePath);

        // 加载状态管理
        bool IsLoaded() const { return m_loadState == LoadState::Loaded; }

        bool IsLoading() const { return m_loadState == LoadState::Loading; }

        bool IsFailed() const { return m_loadState == LoadState::Failed; }

        LoadState GetLoadState() const { return m_loadState; }

        void SetLoadState(LoadState state) { m_loadState = state; }

        // 引用计数管理
        void AddRef() { ++m_refCount; }

        int GetRefCount() const { return m_refCount; }

        void Release(bool force = false);

        virtual long GetSize() const = 0;

        // 序列化支持
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;

        virtual void Deserialize(const rapidjson::Value &json);

        // 依赖关系管理
        const std::vector<std::string> &GetDependencies() const { return m_dependencies; }

        void AddDependency(const std::string &assetId);

        void RemoveDependency(const std::string &assetId);

        void ClearDependencies() { m_dependencies.clear(); }

    public:
#if defined(ZENGINE_EDITOR)

        // 缩略图支持（用于Editor）
        void *GetThumbnailTextureId() const { return m_thumbnailTextureId; }

        // 设置缩略图
        void SetThumbnailTextureId(void *textureId) { m_thumbnailTextureId = textureId; }

        virtual void GenerateThumbnail() = 0;

        void ReleaseThumbnail();

        bool HasThumbnail() const { return m_thumbnailTextureId != nullptr; }

        // 生成唯一的Asset ID
        std::string GenerateAssetId() const;


#endif // ZENGINE_EDITOR

    protected:
#if defined(ZENGINE_EDITOR)
        // 缩略图ID（用于Editor）
        void *m_thumbnailTextureId = nullptr; // 可能是OpenGL纹理ID或其他类型
#endif

    private:
        // 基本信息
        std::string m_name;     // 资源名称
        std::string m_assetId;  // 唯一标识符
        std::string m_filePath; // 文件路径
        AssetType m_type;       // 资源类型

        int m_refCount = 0; // 引用计数
        // 资源加载状态
        LoadState m_loadState = LoadState::NotLoaded;
        // 依赖关系
        std::vector<std::string> m_dependencies;
    };

    // 辅助函数
    std::string AssetTypeToString(AssetType type);

    AssetType StringToAssetType(const std::string &typeStr);
}
