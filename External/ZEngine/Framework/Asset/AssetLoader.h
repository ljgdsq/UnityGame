#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Framework/Asset/Asset.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Util/FileUtil.hpp"
namespace framework
{

    class AssetLoader
    {
    public:
        AssetLoader() = default;
        virtual ~AssetLoader() = default;
        virtual std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) = 0;
        virtual bool CanLoadAsset(const std::string &assetPath) const
        {
            auto extension = FileUtil::GetFileExtension(assetPath);
            for (const auto &ext : GetSupportedExtensions())
            {
                if (FileUtil::HasExtension(assetPath, ext))
                {
                    return true;
                }
            }

            return false;
        }
        virtual std::vector<std::string> GetSupportedExtensions() const = 0;
        virtual std::string GetName() const = 0;
        virtual AssetType GetAssetType() const = 0;
    };

}
