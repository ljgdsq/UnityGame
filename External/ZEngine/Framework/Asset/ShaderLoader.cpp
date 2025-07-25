#include "Framework/Asset/ShaderLoader.h"
#include "Framework/Util/FileUtil.hpp"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Asset/AssetRegister.h"
namespace framework
{
    REGISTER_LOADER(ShaderLoader);
    std::shared_ptr<Asset> ShaderLoader::LoadAsset(const std::string &assetPath)
    {


        Logger::Debug("Loading shader asset from: {}", assetPath);
        std::string newAssetPath = assetPath;
        // 取消扩展名
        if (FileUtil::HasExtension(assetPath, ".vs") || FileUtil::HasExtension(assetPath, ".fs"))
        {
            Logger::Debug("Shader asset path has extension, removing it: {}", assetPath);
            auto lastDot = assetPath.find_last_of('.');
            if (lastDot != std::string::npos)
            {
                newAssetPath = assetPath.substr(0, lastDot);
            }
        }


        // 检查文件是否存在
        if (!EngineFileIO::FileExists(newAssetPath + ".vs") || !EngineFileIO::FileExists(newAssetPath + ".fs"))
        {
            Logger::Error("shader file does not exist: {}", newAssetPath);
            return nullptr;
        }

        // 获取文件名作为资源名称
        std::string assetName = FileUtil::ExtractFileName(newAssetPath);

        // shaderAsset
        auto shaderAsset = std::make_shared<ShaderAsset>(assetName);
        // 设置文件路径
        shaderAsset->SetFilePath(newAssetPath);
        shaderAsset->SetLoadState(LoadState::Loading);

        auto data1 = EngineFileIO::LoadText(newAssetPath + ".vs");
        auto data2 = EngineFileIO::LoadText(newAssetPath + ".fs");

        if (data1.empty() || data2.empty())
        {
            Logger::Error("Failed to load shader source from files: {}.vs or {}.fs", newAssetPath, newAssetPath);
            shaderAsset->SetLoadState(LoadState::Failed);
            return shaderAsset;
        }
        // 创建 Shader 对象
        auto shader = std::make_shared<Shader>();
        shader->Compile(data1.c_str(), data2.c_str());

        shaderAsset->SetShader(shader);

        // 设置加载状态
        shaderAsset->SetLoadState(LoadState::Loaded);

        Logger::Debug("Successfully loaded shader asset: {}", assetName);

        return shaderAsset;
    }

    bool ShaderLoader::CanLoadAsset(const std::string &assetPath) const
    {
        auto extension = FileUtil::GetFileExtension(assetPath);
        if (extension == "vs" || extension == "fs" || extension == "shader")
        {
            return true;
        }
        if (extension.empty())
        {
            // 如果没有扩展名，检查路径是否包含Shaders
            return assetPath.find("Shaders") != std::string::npos;
        }
        return false;
    }

    std::vector<std::string> ShaderLoader::GetSupportedExtensions() const
    {
        return {"vs", "fs", "shader"};
    }

    std::string ShaderLoader::GetName() const
    {
        return "ShaderLoader";
    }

    AssetType ShaderLoader::GetAssetType() const
    {
        return AssetType::Shader;
    }
}
