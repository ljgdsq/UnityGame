#include "Framework/Asset/SceneLoader.h"
#include "Framework/Log/Logger.h"
namespace framework
{

    std::shared_ptr<Asset> SceneLoader::LoadAsset(const std::string &assetPath) {
        Logger::Debug("Loading scene asset from: {}", assetPath);

        // 检查文件是否存在
        if (!EngineFileIO::FileExists(assetPath)) {
            Logger::Error("Scene asset not found: {}", assetPath);
            return nullptr;
        }


        // 获取文件名作为资源名称
        std::string assetName = FileUtil::ExtractFileName(assetPath);

        auto sceneAsset = std::make_shared<SceneAsset>(assetName);
        sceneAsset->SetFilePath(assetPath);
        sceneAsset->SetLoadState(LoadState::Loading);

        auto data = EngineFileIO::LoadText(assetPath);
        if (data.empty()) {
            Logger::Error("Failed to load scene data from file: {}", assetPath);
            sceneAsset->SetLoadState(LoadState::Failed);
            return sceneAsset;
        }

        try
        {
            rapidjson::Document doc;
            auto scene = std::make_shared<Scene>();
            doc.Parse(data.c_str());
            if (doc.HasParseError()) {
                Logger::Error("Failed to parse scene data: {}",(int)doc.GetParseError());
                sceneAsset->SetLoadState(LoadState::Failed);
                return sceneAsset;
            }

            scene->Deserialize(doc); // 假设 Scene 类有 Deserialize 方法来解析数据
            sceneAsset->SetScene(scene);
        }
        catch(const std::exception& e)
        {
           Logger::Error("Failed to deserialize scene data: {}", e.what());
           sceneAsset->SetLoadState(LoadState::Failed);
        }
        sceneAsset->SetLoadState(LoadState::Loaded);
        Logger::Debug("Successfully loaded scene asset: {}", assetName);

        return sceneAsset;
    }

    bool SceneLoader::CanLoadAsset(const std::string &assetPath) const {
        return AssetLoader::CanLoadAsset(assetPath);
    }

    std::vector<std::string> SceneLoader::GetSupportedExtensions() const {
        return {".scene"};
    }

    std::string SceneLoader::GetName() const {
        return "SceneLoader";
    }

    AssetType SceneLoader::GetAssetType() const {
        return AssetType::Scene;
    }
} // namespace framework
