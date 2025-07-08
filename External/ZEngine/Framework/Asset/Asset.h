#pragma once
#include <string>
namespace framework
{
    enum class AssetType
    {
        Unknown = 0, // 未知类型
        Texture,
        Shader,
        Mesh,
        Audio,
        Font,
        Count // 用于计数
    };

    class Asset
    {
    public:
        Asset(const std::string &name,AssetType type)
            : m_type(type), m_name(name) {}
        virtual ~Asset() = default;
        // basae info
        AssetType GetType() const { return m_type; }
        const std::string &GetName() const { return m_name; }
        // 是否已加载
        bool IsLoaded() const { return m_isLoaded; }
        // 设置加载状态
        void SetLoaded(bool loaded) { m_isLoaded = loaded; }

        // 引用计数管理
        void AddRef()
        {
            ++m_refCount;
        }
        void Release()
        {
            if (--m_refCount <= 0)
            {
                delete this; // 删除自身
            }
        }

        virtual void Load() = 0;          // 纯虚函数，子类必须实现
        virtual void Unload() = 0;        // 纯虚函数，子类必须实现
        virtual long GetSize() const = 0; // 获取资源大小，单位字节

    private:
        int m_refCount = 0; // 引用计数
                            // 资源加载状态
        bool m_isLoaded = false; // 是否已加载
        AssetType m_type;        // 资源类型
        std::string m_name;      // 资源名称
    };

}