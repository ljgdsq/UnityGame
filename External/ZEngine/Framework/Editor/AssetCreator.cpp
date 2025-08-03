#include "Framework/Editor/AssetCreator.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Util/FileUtil.hpp"
#include "Framework/Util/JsonUtil.hpp"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Asset/SceneAsset.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Asset/TextureAsset.h"
#include <filesystem>
using namespace framework;

namespace editor
{
    void AssetCreator::CreateMaterialAsset(const std::string &name)
    {
        std::filesystem::path materialPath("Res/Materials/");
        if (!std::filesystem::exists(materialPath))
        {
            std::filesystem::create_directories(materialPath);
        }

        std::string filePath = (materialPath / name).string();
        auto fileName = framework::FileUtil::ExtractFileName(name);

        auto materialAsset = std::make_shared<framework::MaterialAsset>(fileName);
        materialAsset->SetFilePath(filePath);
        rapidjson::Document::AllocatorType allocator;
        auto value = materialAsset->Serialize(allocator);
        auto str = framework::JsonUtil::JsonToString(value);
        if (!framework::EngineFileIO::SaveText(filePath, str))
        {
            Logger::Error("Failed to save material asset to {}", filePath);
            return;
        }
    }

    void AssetCreator::CreateTemplateScene(const std::string &name)
    {
        std::filesystem::path scenePath("Res/Scenes/");
        if (!std::filesystem::exists(scenePath))
        {
            std::filesystem::create_directories(scenePath);
        }

        std::filesystem::path templateScenePath("Res/Internal/Scenes/NewScene.scene");

        std::string filePath = (scenePath / name).string();
        auto fileName = framework::FileUtil::ExtractFileName(name);

        if (!std::filesystem::exists(templateScenePath))
        {
            Logger::Error("Template scene file does not exist: {}", templateScenePath.string());
            return;
        }

        // 复制模板场景文件到新路径
        try
        {
            std::filesystem::copy(templateScenePath, filePath, std::filesystem::copy_options::overwrite_existing);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            Logger::Error("Failed to copy template scene: {}", e.what());
            return;
        }
    }

    void AssetCreator::CreateEmptyScene(const std::string &name)
    {
        std::filesystem::path scenePath("Res/Scenes/");
        if (!std::filesystem::exists(scenePath))
        {
            std::filesystem::create_directories(scenePath);
        }
        std::string filePath = (scenePath / name).string();
        auto fileName = framework::FileUtil::ExtractFileName(name);

        auto sceneAsset = std::make_shared<framework::SceneAsset>(fileName);
        sceneAsset->SetFilePath(filePath);
        rapidjson::Document::AllocatorType allocator;
        auto scene = std::make_shared<framework::Scene>();
        scene->SetName(fileName);
        {
            // todo
        }

        auto value = sceneAsset->Serialize(allocator);
        auto str = framework::JsonUtil::JsonToString(value);
        if (!framework::EngineFileIO::SaveText(filePath, str))
        {
            Logger::Error("Failed to save scene asset to {}", filePath);
            return;
        }
    }
} // namespace editor
