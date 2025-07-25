#include "Framework/Editor/AssetCreator.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Util/FileUtil.hpp"
#include "Framework/Util/JsonUtil.hpp"
#include "Framework/Asset/MaterialAsset.h"
#include <filesystem>
namespace editor
{
    void AssetCreator::CreateMaterialAsset(const std::string &name)
    {
        std::filesystem::path materialPath("Res/Materials/");
        if (!std::filesystem::exists(materialPath))
        {
            std::filesystem::create_directories(materialPath);
        }

        std::string filePath = (materialPath / name).string() ;
        auto fileName = framework::FileUtil::ExtractFileName(name);

        auto materialAsset = std::make_shared<framework::MaterialAsset>(fileName);
        materialAsset->SetFilePath(filePath);
        rapidjson::Document::AllocatorType &allocator = rapidjson::Document::AllocatorType();
        auto value = materialAsset->Serialize(allocator);
        auto str = framework::JsonUtil::JsonToString(value);
        if (!framework::EngineFileIO::SaveText(filePath, str))
        {
            Logger::Error("Failed to save material asset to {}", filePath);
            return;
        }
    }
} // namespace editor
