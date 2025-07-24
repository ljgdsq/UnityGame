#pragma once
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Asset/SceneAsset.h"
namespace framework
{
    class SceneLoader : public AssetLoader
    {
    public:
        SceneLoader() = default;
        ~SceneLoader() = default;

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) override;

        bool CanLoadAsset(const std::string &assetPath) const override;

        std::vector<std::string> GetSupportedExtensions() const override;

        std::string GetName() const override;

        AssetType GetAssetType() const override;

    private:
        // 当前加载的场景
        std::unique_ptr<SceneAsset> m_currentScene;
    };
} // namespace framework


