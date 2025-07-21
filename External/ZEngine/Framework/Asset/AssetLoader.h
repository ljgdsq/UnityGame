#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Framework/Asset/Asset.h"
namespace framework
{
    enum class LoadState
    {
        NotLoaded = 0,
        Loading,
        Loaded,
        Failed
    };

    class AssetLoader
    {
    public:
        AssetLoader() = default;
        virtual ~AssetLoader() = default;

        virtual std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) = 0;
        virtual bool CanLoadAsset(const std::string &assetPath) const = 0;
        virtual std::vector<std::string> GetSupportedExtensions() const = 0;
        virtual std::string GetName() const = 0;
        virtual AssetType GetAssetType() const = 0;

        // 加载状态管理
        bool IsLoaded() const { return m_loadState == LoadState::Loaded; }
        bool IsLoading() const { return m_loadState == LoadState::Loading; }
        bool HasFailed() const { return m_loadState == LoadState::Failed; }
        LoadState GetLoadState() const { return m_loadState; }

        // 内部使用的状态设置
        void SetLoadState(LoadState state) { m_loadState = state; }
        protected:
            // 状态管理
            LoadState m_loadState = LoadState::NotLoaded;
    };
}
