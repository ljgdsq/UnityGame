#pragma once
#include "Framework/Asset/AssetLoader.h"
namespace framework{
    class ShaderLoader:public AssetLoader{
    public:
        ~ShaderLoader() override = default;

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) override;

        bool CanLoadAsset(const std::string &assetPath) const override;

        std::vector<std::string> GetSupportedExtensions() const override;

        std::string GetName() const override;

        AssetType GetAssetType() const override;
    };


}
