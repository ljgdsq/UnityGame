#pragma once
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Asset/TextureAsset.h"
#include <string>
#include <memory>

namespace framework
{
    class TextureLoader : public AssetLoader
    {
    public:
        TextureLoader() = default;
        ~TextureLoader() override = default;

        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) override;
        bool CanLoadAsset(const std::string &assetPath) const override;
        std::vector<std::string> GetSupportedExtensions() const override;
        std::string GetName() const override;
        AssetType GetAssetType() const override { return AssetType::Texture; }

    private:
        bool GenTexture(const std::vector<uint8_t> &data, std::shared_ptr<TextureAsset> asset);
    };
}
