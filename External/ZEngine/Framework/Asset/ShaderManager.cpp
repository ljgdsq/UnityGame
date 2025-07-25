#include "Framework/Asset/ShaderManager.h"
#include "Framework/Asset/AssetManager.h"

namespace framework
{
    void ShaderManager::Initialize()
    {
        Logger::Log("ShaderManager initialized");
        // 预加所有Shaders目录下的着色器

        // 所有的.vs .fs
        auto files = EngineFileIO::ListFilesInDirectory("Res/Shaders");
        for (const auto &file : files)
        {
            if (file.extension() == ".vs" || file.extension() == ".fs")
            {

                auto shaderAsset = AssetManager::GetInstance().LoadAsset<ShaderAsset>(file.string());
                if (shaderAsset && !shaderAsset->IsFailed())
                {
                    m_shaderCache[file.stem().string()] = shaderAsset;
                }
                else
                {
                    Logger::Error("Failed to load shader asset: {}", file.stem().string());
                }
            }
        }

        Logger::Debug("ShaderManager initialized with {} shaders", m_shaderCache.size());

        // 输出已加载的着色器名称
        for (const auto &pair : m_shaderCache)
        {
            Logger::Log("Loaded shader: {}", pair.first);
        }
    }

    std::shared_ptr<ShaderAsset> ShaderManager::GetShader(const std::string &name)
    {
        if (m_shaderCache.find(name) != m_shaderCache.end())
        {
            return m_shaderCache[name];
        }

        auto asset = AssetManager::GetInstance().LoadAsset<ShaderAsset>(name);
        if (asset && !asset->IsFailed())
        {
            m_shaderCache[name] = asset;
            return asset;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<ShaderAsset>> ShaderManager::GetAllShaders()
    {
        std::vector<std::shared_ptr<ShaderAsset>> shaders;
        for (const auto &pair : m_shaderCache)
        {
            shaders.push_back(pair.second);
        }
        return shaders;
    }
} // namespace framework
