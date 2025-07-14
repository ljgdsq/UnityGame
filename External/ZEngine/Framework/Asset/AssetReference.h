#pragma once
#include <string>
#include <memory>
#include <functional>
#include "Framework/Asset/Asset.h"
#include "rapidjson/document.h"

namespace framework
{
    template <typename AssetType>
    class AssetReference
    {
    public:
        AssetReference() = default;

        explicit AssetReference(const std::string &assetId)
            : m_assetId(assetId) {}

        explicit AssetReference(std::shared_ptr<AssetType> asset)
            : m_cachedAsset(asset)
        {
            if (asset)
            {
                m_assetId = asset->GetAssetId();
            }
        }

        ~AssetReference() = default;

        // 拷贝构造和赋值
        AssetReference(const AssetReference &other) = default;
        AssetReference &operator=(const AssetReference &other) = default;

        // 移动构造和赋值
        AssetReference(AssetReference &&other) noexcept = default;
        AssetReference &operator=(AssetReference &&other) noexcept = default;

        // 赋值操作
        AssetReference &operator=(const std::string &assetId)
        {
            m_assetId = assetId;
            m_cachedAsset.reset();
            return *this;
        }

        AssetReference &operator=(std::shared_ptr<AssetType> asset)
        {
            m_cachedAsset = asset;
            if (asset)
            {
                m_assetId = asset->GetAssetId();
            }
            else
            {
                m_assetId.clear();
            }
            return *this;
        }

        // 访问操作
        std::shared_ptr<AssetType> Get() const
        {
            // 如果缓存有效，直接返回
            if (auto asset = m_cachedAsset.lock())
            {
                return asset;
            }

            // 如果有Asset ID，尝试加载
            if (!m_assetId.empty())
            {
                auto asset = LoadAsset();
                if (asset)
                {
                    m_cachedAsset = asset;
                    return asset;
                }
            }

            // 返回默认资源
            return GetDefaultAsset();
        }

        std::shared_ptr<AssetType> operator->() const
        {
            return Get();
        }

        AssetType &operator*() const
        {
            auto asset = Get();
            return *asset;
        }

        // 状态查询
        bool IsValid() const
        {
            return !m_assetId.empty() || !m_cachedAsset.expired();
        }

        bool IsLoaded() const
        {
            if (auto asset = m_cachedAsset.lock())
            {
                return asset->IsLoaded();
            }
            return false;
        }

        bool IsLoading() const
        {
            if (auto asset = m_cachedAsset.lock())
            {
                return asset->IsLoading();
            }
            return false;
        }

        const std::string &GetAssetId() const
        {
            return m_assetId;
        }

        // 强制重新加载
        void Reload()
        {
            m_cachedAsset.reset();
            if (!m_assetId.empty())
            {
                auto asset = LoadAsset();
                if (asset)
                {
                    m_cachedAsset = asset;
                }
            }
        }

        // 清空引用
        void Clear()
        {
            m_assetId.clear();
            m_cachedAsset.reset();
        }

        // 序列化支持
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const
        {
            rapidjson::Value jsonValue(rapidjson::kObjectType);
            jsonValue.AddMember("assetId", rapidjson::Value(m_assetId.c_str(), allocator), allocator);
            return jsonValue;
        }

        void Deserialize(const rapidjson::Value &json)
        {
            if (json.HasMember("assetId") && json["assetId"].IsString())
            {
                m_assetId = json["assetId"].GetString();
                m_cachedAsset.reset(); // 清空缓存，等待重新加载
            }
        }

        // 异步加载
        void LoadAsync(std::function<void(std::shared_ptr<AssetType>)> callback = nullptr)
        {
            if (m_assetId.empty())
            {
                if (callback)
                {
                    callback(GetDefaultAsset());
                }
                return;
            }

            LoadAssetAsync(m_assetId, [this, callback](std::shared_ptr<Asset> asset)
                           {
                if (asset)
                {
                    auto typedAsset = std::static_pointer_cast<AssetType>(asset);
                    m_cachedAsset = typedAsset;
                    if (callback)
                    {
                        callback(typedAsset);
                    }
                }
                else if (callback)
                {
                    callback(GetDefaultAsset());
                } });
        }

        // 比较操作
        bool operator==(const AssetReference &other) const
        {
            return m_assetId == other.m_assetId;
        }

        bool operator!=(const AssetReference &other) const
        {
            return !(*this == other);
        }

        // 显式转换为bool
        explicit operator bool() const
        {
            return IsValid();
        }

    private:
        std::string m_assetId;
        mutable std::weak_ptr<AssetType> m_cachedAsset;

        // 同步加载Asset
        std::shared_ptr<AssetType> LoadAsset() const;

        // 异步加载Asset
        void LoadAssetAsync(const std::string &assetId, std::function<void(std::shared_ptr<Asset>)> callback) const;

        // 获取默认Asset
        std::shared_ptr<AssetType> GetDefaultAsset() const;
    };

    // 类型别名
    template <typename T>
    using AssetRef = AssetReference<T>;

} // namespace framework

// 包含实现文件
#include "Framework/Asset/AssetReference.inl"
