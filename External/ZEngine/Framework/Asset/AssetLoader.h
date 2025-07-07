#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Framework/Asset/Asset.h"
namespace framework
{
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
    };
}