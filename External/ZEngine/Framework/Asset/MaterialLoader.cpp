#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Asset/MaterialLoader.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetRegister.h"

namespace framework
{
    REGISTER_LOADER(MaterialLoader);

    std::vector<std::string> MaterialLoader::GetSupportedExtensions() const
    {
        return {".material", ".mat"};
    }

    std::shared_ptr<Asset> MaterialLoader::LoadAsset(const std::string &assetPath)
    {
        Logger::Debug("Loading Material from path: {}", assetPath);

        std::string assetName = FileUtil::ExtractFileName(assetPath);
        auto materialAsset = std::make_shared<MaterialAsset>(assetName);

        // 检查文件是否存在
        if (!EngineFileIO::FileExists(assetPath))
        {
            materialAsset->SetLoadState(LoadState::Failed);
            Logger::Error("Material file does not exist: {}", assetPath);
            return materialAsset;
        }

        // 设置文件路径和加载状态
        materialAsset->SetFilePath(assetPath);
        materialAsset->SetLoadState(LoadState::Loading);

        // 读取材质数据（假设是JSON格式）
        rapidjson::Document doc;
        auto jsonText = EngineFileIO::LoadText(assetPath);
        if (jsonText.empty())
        {
            materialAsset->SetLoadState(LoadState::Failed);
            Logger::Error("Failed to read Material file: {}", assetPath);
            return materialAsset;
        }
        doc.Parse(jsonText.c_str());

        // 反序列化材质数据
        materialAsset->Deserialize(doc);

        // 设置加载状态为已加载
        materialAsset->SetLoadState(LoadState::Loaded);
        Logger::Log("Successfully loaded Material: {}", assetName);

        return materialAsset;
    }

} // namespace framework
