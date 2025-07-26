#include "Framework/Asset/UnknownAssetLoader.h"
#include "Framework/Log/Logger.h"
#include "Framework/Util/FileUtil.hpp"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Asset/UnknownAsset.h"
namespace framework
{

    std::shared_ptr<Asset> UnknownAssetLoader::LoadAsset(const std::string &assetPath)
    {
        LOG_DEBUG("Attempting to load unknown asset type from: {}", assetPath);

        if (!EngineFileIO::FileExists(assetPath))
        {
            LOG_ERROR("Asset file does not exist: {}", assetPath);
            return nullptr;
        }

        std::string assetName = FileUtil::ExtractFileName(assetPath);

        // If the file exists, create an UnknownAsset instance
        auto asset = std::make_shared<UnknownAsset>(assetName);
        asset->SetFilePath(assetPath);
        asset->SetLoadState(LoadState::Loading);

        auto size=EngineFileIO::GetFileSize(assetPath);
        asset->SetFileSize(size);
        asset->SetLoadState(LoadState::NotLoaded);

        return asset;
    }

    bool UnknownAssetLoader::CanLoadAsset(const std::string &assetPath) const
    {
        return true;
    }

    std::vector<std::string> UnknownAssetLoader::GetSupportedExtensions() const
    {
        return std::vector<std::string>();
    }

    std::string UnknownAssetLoader::GetName() const
    {
        return "UnknownAssetLoader";
    }

    AssetType UnknownAssetLoader::GetAssetType() const
    {
        return AssetType::Unknown;
    }
}
