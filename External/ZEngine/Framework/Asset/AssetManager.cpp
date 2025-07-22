#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Asset/ObjMeshLoader.h"
#include "Framework/Asset/TextureLoader.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Util/FileUtil.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

namespace framework
{
    AssetManager *AssetManager::s_instance = nullptr;

    // 支持的文件扩展名映射（从AssetRegistry迁移）
    const std::unordered_map<std::string, AssetType> AssetManager::s_supportedExtensions = {
        // 纹理文件
        {".png", AssetType::Texture},
        {".jpg", AssetType::Texture},
        {".jpeg", AssetType::Texture},
        {".bmp", AssetType::Texture},
        {".tga", AssetType::Texture},
        {".dds", AssetType::Texture},

        // 网格文件
        {".obj", AssetType::Mesh},
        {".fbx", AssetType::Mesh},
        {".dae", AssetType::Mesh},
        {".3ds", AssetType::Mesh},
        {".blend", AssetType::Mesh},

        // 着色器文件
        {".vs", AssetType::Shader},
        {".fs", AssetType::Shader},
        {".gs", AssetType::Shader},
        {".hlsl", AssetType::Shader},
        {".glsl", AssetType::Shader},

        // 音频文件
        {".wav", AssetType::Audio},
        {".mp3", AssetType::Audio},
        {".ogg", AssetType::Audio},
        {".m4a", AssetType::Audio},

        // 字体文件
        {".ttf", AssetType::Font},
        {".otf", AssetType::Font},

        // 材质文件
        {".mat", AssetType::Material},

        // 动画文件
        {".anim", AssetType::Animation}};

    AssetManager &AssetManager::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = new AssetManager();
            Logger::Log("AssetManager instance created");
        }
        return *s_instance;
    }

    AssetManager::~AssetManager()
    {
        UnloadAllAssets();
        m_assets.clear();
        m_loaders.clear();
        Logger::Log("AssetManager destroyed and all assets unloaded");
    }

    void AssetManager::RegisterLoader(std::shared_ptr<AssetLoader> loader)
    {
        std::lock_guard<std::mutex> lock(m_loaderMutex);

        // 检查是否已经注册了同名加载器
        auto it = std::find_if(m_loaders.begin(), m_loaders.end(),
                               [&loader](const std::shared_ptr<AssetLoader> &existingLoader)
                               {
                                   return existingLoader->GetName() == loader->GetName();
                               });

        if (it != m_loaders.end())
        {
            Logger::Warn("Asset loader '{}' is already registered, skipping registration", loader->GetName());
            return;
        }

        m_loaders.push_back(loader);
        Logger::Log("Registered asset loader: {}", loader->GetName());
        auto extensions = loader->GetSupportedExtensions();
        std::string supportedExtensions;
        for (const auto &ext : extensions)
        {
            if (!supportedExtensions.empty())
            {
                supportedExtensions += ", ";
            }
            supportedExtensions += ext;
        }

        Logger::Log("Loader '{}' supports extensions: {}", loader->GetName(), supportedExtensions);
    }

    void AssetManager::UnregisterLoader(std::shared_ptr<AssetLoader> loader)
    {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        auto it = std::remove(m_loaders.begin(), m_loaders.end(), loader);
        if (it != m_loaders.end())
        {
            m_loaders.erase(it, m_loaders.end());
            Logger::Log("Unregistered asset loader: {}", loader->GetName());
        }
        else
        {
            Logger::Warn("Attempted to unregister non-existent asset loader: {}", loader->GetName());
        }
    }

    template <class T>
    std::shared_ptr<T> AssetManager::LoadAsset(const std::string &assetPath)
    {
        // 首先尝试从已加载的资源中获取
        std::string assetName = FileUtil::ExtractFileName(assetPath);
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assets.find(assetName);
            if (it != m_assets.end())
            {
                auto asset = std::dynamic_pointer_cast<T>(it->second);
                if (asset)
                {
                    asset->AddRef(); // 增加引用计数
                    // Logger::Log("Asset '{}' already loaded, returning existing instance", assetName);
                    return asset;
                }
                else
                {
                    Logger::Error("Asset '{}' is not of type {}", assetName, typeid(T).name());
                    return nullptr;
                }
            }
        }

        // 加载新资源 - 调用带类型参数的版本,让它自动检测类型
        auto asset = LoadAsset(assetPath, AssetType::Unknown);
        if (asset)
        {
            return std::dynamic_pointer_cast<T>(asset);
        }
        return nullptr;
    }

    std::shared_ptr<Asset> AssetManager::LoadAsset(const std::string &assetPath, AssetType type)
    {
        Logger::Debug("Loading asset from path: {}", assetPath);

        std::string assetName = FileUtil::ExtractFileName(assetPath);
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assets.find(assetName);
            if (it != m_assets.end())
            {
                Logger::Log("Asset '{}' already loaded, returning existing instance", assetName);
                return it->second; // 返回已加载的资源
            }
        }

        // 检查文件是否存在
        if (!EngineFileIO::FileExists(assetPath))
        {
            Logger::Error("Asset file does not exist: {}", assetPath);
            return nullptr;
        }

        // 查找合适的加载器
        AssetLoader *loader = FindLoader(assetPath);
        if (!loader)
        {
            Logger::Error("No suitable asset loader found for path: {}", assetPath);
            return nullptr;
        }

        Logger::Debug("Using loader '{}' to load asset: {}", loader->GetName(), assetName);
        std::shared_ptr<Asset> asset = loader->LoadAsset(assetPath);
        if (!asset)
        {
            Logger::Error("Failed to load asset from path: {}", assetPath);
            return nullptr;
        }

        // check asset
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_assets[assetName] = asset;               // 存储已加载的资源
            m_assetsById[asset->GetAssetId()] = asset; // 同时按 ID 存储
        }
        Logger::Log("Successfully loaded asset: {}", assetName);
        return asset;
    }

    std::shared_ptr<Asset> AssetManager::GetAsset(const std::string &assetName)
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assets.find(assetName);
        if (it != m_assets.end())
        {
            return it->second; // 返回已加载的资源
        }
        Logger::Warn("Asset '{}' not found", assetName);
        return nullptr;
    }

    bool AssetManager::HasAsset(const std::string &assetName) const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        return m_assets.find(assetName) != m_assets.end();
    }

    void AssetManager::UnloadAsset(const std::string &assetName)
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assets.find(assetName);
        if (it != m_assets.end())
        {
            it->second->Unload();  // 调用资源的卸载方法
            it->second->Release(); // 减少引用计数
            m_assets.erase(it);    // 从集合中移除
            Logger::Log("Asset '{}' unloaded successfully", assetName);
        }
        else
        {
            Logger::Warn("Attempted to unload non-existent asset: {}", assetName);
        }
    }

    void AssetManager::UnloadAllAssets()
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        for (auto &pair : m_assets)
        {
            pair.second->Unload();  // 调用资源的卸载方法
            pair.second->Release(); // 减少引用计数
        }
        m_assets.clear(); // 清空已加载的资源集合
        Logger::Log("All assets unloaded successfully");
    }

    void AssetManager::PreloadAssets(const std::vector<std::string> &assetPaths)
    {
        Logger::Error("PreloadAssets is not implemented yet");
    }

    long AssetManager::GetTotalAssetsSize() const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        long totalSize = 0;
        for (const auto &pair : m_assets)
        {
            totalSize += pair.second->GetSize(); // 累加每个资源的大小
        }
        return totalSize;
    }

    long AssetManager::GetTotalAssetsCount() const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        return m_assets.size(); // 返回已加载资源的数量
    }

    std::vector<std::string> AssetManager::GetAllAssetNames() const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        std::vector<std::string> assetNames;
        for (const auto &pair : m_assets)
        {
            assetNames.push_back(pair.first); // 收集所有资源的名称
        }
        return assetNames;
    }

    AssetLoader *AssetManager::FindLoader(const std::string &assetPath) const
    {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        for (const auto &loader : m_loaders)
        {
            if (loader->CanLoadAsset(assetPath))
            {
                return loader.get(); // 返回找到的加载器
            }
        }
        return nullptr; // 没有找到合适的加载器
    }

    AssetLoader *AssetManager::FindLoaderByType(AssetType type) const
    {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        for (const auto &loader : m_loaders)
        {
            if (loader->GetAssetType() == type)
            {
                return loader.get(); // 返回找到的加载器
            }
        }
        return nullptr; // 没有找到合适的加载器
    }

    void AssetManager::CheckForChanges()
    {
    }

    // === 新增的方法实现 ===

    void AssetManager::Initialize()
    {
        Logger::Debug("Initializing AssetManager...");

        // 注册默认资源加载器
        RegisterLoader(std::make_shared<ObjMeshLoader>());
        RegisterLoader(std::make_shared<TextureLoader>());

        // 初始化资源扫描
        ScanResourceDirectories();

        // 初始化默认资源
        InitializeDefaultAssets();

        // 启动异步加载
        StartAsyncLoading();

        Logger::Debug("AssetManager initialized successfully.");
    }

    void AssetManager::Shutdown()
    {
        Logger::Debug("Shutting down AssetManager...");

        // 停止异步加载
        StopAsyncLoading();

        // 卸载所有资源
        UnloadAllAssets();

        // 清理默认资源
        m_defaultTexture.reset();
        m_defaultMesh.reset();
        m_defaultMaterial.reset();

        Logger::Debug("AssetManager shutdown completed.");
    }

    std::shared_ptr<Asset> AssetManager::LoadAsset(const std::string &assetId)
    {
        // 先检查是否已经加载
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assetsById.find(assetId);
            if (it != m_assetsById.end())
            {
                return it->second;
            }
        }

        // 从资源信息获取资源信息
        const AssetInfo *info = GetAssetInfo(assetId);
        if (!info)
        {
            Logger::Error("Asset not found in registry: {}", assetId);
            return nullptr;
        }

        // 同步加载资源
        return LoadAssetSync(assetId);
    }

    void AssetManager::LoadAssetAsync(const std::string &assetId,
                                      std::function<void(std::shared_ptr<Asset>)> callback,
                                      LoadPriority priority)
    {
        // 先检查是否已经加载
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assetsById.find(assetId);
            if (it != m_assetsById.end())
            {
                if (callback)
                {
                    callback(it->second);
                }
                return;
            }
        }

        // 添加到加载队列
        LoadRequest request;
        request.assetId = assetId;
        request.priority = priority;
        request.callback = callback;

        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_loadQueue.push(request);
        }

        m_queueCondition.notify_one();
    }

    std::shared_ptr<Asset> AssetManager::GetAssetById(const std::string &assetId)
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assetsById.find(assetId);
        return (it != m_assetsById.end()) ? it->second : nullptr;
    }

    bool AssetManager::HasAssetById(const std::string &assetId) const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        return m_assetsById.find(assetId) != m_assetsById.end();
    }

    void AssetManager::UnloadAssetById(const std::string &assetId)
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assetsById.find(assetId);
        if (it != m_assetsById.end())
        {
            it->second->Unload();
            m_assetsById.erase(it);
            Logger::Debug("Unloaded asset by ID: {}", assetId);
        }
    }

    std::shared_ptr<TextureAsset> AssetManager::GetDefaultTextureAsset()
    {
        if (!m_defaultTexture)
        {
            CreateDefaultTexture();
        }
        return m_defaultTexture;
    }

    std::shared_ptr<MeshAsset> AssetManager::GetDefaultMeshAsset()
    {
        if (!m_defaultMesh)
        {
            CreateDefaultMesh();
        }
        return m_defaultMesh;
    }

    std::shared_ptr<TextureAsset> AssetManager::CreateTextureAsset(const std::string &name)
    {
        auto textureAsset = std::make_shared<TextureAsset>(name);

        // 注册到AssetManager的内部存储
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_assets[name] = textureAsset;
            m_assetsById[textureAsset->GetAssetId()] = textureAsset;
        }

        Logger::Debug("Created and registered TextureAsset: {} (ID: {})", name, textureAsset->GetAssetId());
        return textureAsset;
    }

    std::shared_ptr<MeshAsset> AssetManager::CreateMeshAsset(const std::string &name)
    {
        auto meshAsset = std::make_shared<MeshAsset>(name);

        // 注册到AssetManager的内部存储
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_assets[name] = meshAsset;
            m_assetsById[meshAsset->GetAssetId()] = meshAsset;
        }

        Logger::Debug("Created and registered MeshAsset: {} (ID: {})", name, meshAsset->GetAssetId());
        return meshAsset;
    }

    std::shared_ptr<Asset> AssetManager::CreateAsset(const std::string &name, AssetType type)
    {
        switch (type)
        {
        case AssetType::Texture:
            return CreateTextureAsset(name);
        case AssetType::Mesh:
            return CreateMeshAsset(name);
        // 其他类型的Asset可以在这里添加
        default:
            Logger::Error("Unsupported asset type for creation: {}", static_cast<int>(type));
            return nullptr;
        }
    }

    void AssetManager::RegisterAsset(std::shared_ptr<Asset> asset)
    {
        if (!asset)
        {
            Logger::Error("Cannot register null asset");
            return;
        }

        std::lock_guard<std::mutex> lock(m_assetMutex);
        m_assets[asset->GetName()] = asset;
        m_assetsById[asset->GetAssetId()] = asset;

        Logger::Debug("Registered asset: {} (ID: {})", asset->GetName(), asset->GetAssetId());
    }

    void AssetManager::InitializeDefaultAssets()
    {
        Logger::Debug("Initializing default assets...");

        CreateDefaultTexture();
        CreateDefaultMesh();

        Logger::Debug("Default assets initialized.");
    }

    void AssetManager::CreateDefaultTexture()
    {
        m_defaultTexture = std::make_shared<TextureAsset>("DefaultTexture");
        m_defaultTexture->CreateEmpty(64, 64, 4); // 64x64 白色纹理
        m_defaultTexture->SetLoadState(LoadState::Loaded);
        Logger::Debug("Created default texture.");
    }

    void AssetManager::CreateDefaultMesh()
    {
        m_defaultMesh = std::make_shared<MeshAsset>("DefaultMesh");

        // 创建一个简单的四边形网格
        auto mesh = std::make_shared<Mesh>();

        // 四边形顶点数据 (位置 + 法线 + 纹理坐标)
        std::vector<float> vertices = {
            // 位置            // 法线          // 纹理坐标
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

        // 索引数据
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0};

        mesh->SetVertices(vertices);
        mesh->SetIndices(indices);

        m_defaultMesh->SetMesh(mesh);
        m_defaultMesh->SetLoadState(LoadState::Loaded);
        Logger::Debug("Created default mesh.");
    }

    void AssetManager::StartAsyncLoading()
    {
        if (m_asyncLoadingEnabled)
        {
            return;
        }

        m_asyncLoadingEnabled = true;

        // 创建加载线程
        size_t threadCount = std::max(1u, std::thread::hardware_concurrency() / 2);
        m_loadingThreads.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i)
        {
            m_loadingThreads.emplace_back(&AssetManager::LoadingWorker, this);
        }

        Logger::Debug("Started {} async loading threads.", threadCount);
    }

    void AssetManager::StopAsyncLoading()
    {
        if (!m_asyncLoadingEnabled)
        {
            return;
        }

        m_asyncLoadingEnabled = false;
        m_queueCondition.notify_all();

        // 等待所有线程结束
        for (auto &thread : m_loadingThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        m_loadingThreads.clear();
        Logger::Debug("Stopped async loading threads.");
    }

    void AssetManager::LoadingWorker()
    {
        while (m_asyncLoadingEnabled)
        {
            LoadRequest request;

            // 从队列中获取加载请求
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_queueCondition.wait(lock, [this]
                                      { return !m_loadQueue.empty() || !m_asyncLoadingEnabled; });

                if (!m_asyncLoadingEnabled)
                {
                    break;
                }

                if (!m_loadQueue.empty())
                {
                    request = m_loadQueue.top();
                    m_loadQueue.pop();
                }
                else
                {
                    continue;
                }
            }

            // 执行加载
            auto asset = LoadAssetSync(request.assetId);

            // 回调通知
            if (request.callback)
            {
                request.callback(asset);
            }
        }
    }

    std::shared_ptr<Asset> AssetManager::LoadAssetSync(const std::string &assetId)
    {
        // 从资源信息获取资源信息
        const AssetInfo *info = GetAssetInfo(assetId);
        if (!info)
        {
            Logger::Error("Asset not found in registry: {}", assetId);
            return nullptr;
        }

        try
        {
            // 根据类型创建Asset
            std::shared_ptr<Asset> asset;
            switch (info->type)
            {
            case AssetType::Texture:
                asset = std::make_shared<TextureAsset>(info->displayName, assetId);
                break;
            case AssetType::Mesh:
                asset = std::make_shared<MeshAsset>(info->displayName);
                break;
            default:
                Logger::Error("Unsupported asset type: {}", static_cast<int>(info->type));
                return nullptr;
            }

            // 设置文件路径并加载
            asset->SetFilePath(info->filePath);
            asset->Load();

            // 添加到缓存
            {
                std::lock_guard<std::mutex> lock(m_assetMutex);
                m_assetsById[assetId] = asset;
                m_assets[info->displayName] = asset;
            }

            Logger::Debug("Loaded asset: {} (ID: {})", info->displayName, assetId);
            return asset;
        }
        catch (const std::exception &e)
        {
            Logger::Error("Exception while loading asset {}: {}", assetId, e.what());
            return nullptr;
        }
    }

    std::vector<std::string> AssetManager::GetAllAssetIds() const
    {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        std::vector<std::string> assetIds;
        assetIds.reserve(m_assetsById.size());

        for (const auto &pair : m_assetsById)
        {
            assetIds.push_back(pair.first);
        }

        return assetIds;
    }

    // 显式模板实例化
    template std::shared_ptr<TextureAsset> AssetManager::LoadAsset<TextureAsset>(const std::string &assetId);
    template std::shared_ptr<MeshAsset> AssetManager::LoadAsset<MeshAsset>(const std::string &assetId);

    // ===== 以下是从AssetRegistry合并的功能实现 =====

    void AssetManager::RegisterAssetInfo(const AssetInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        m_assetInfos[info.assetId] = info;
        Logger::Debug("Registered asset info: {} (ID: {})", info.displayName, info.assetId);
    }

    void AssetManager::UnregisterAssetInfo(const std::string &assetId)
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        auto it = m_assetInfos.find(assetId);
        if (it != m_assetInfos.end())
        {
            Logger::Debug("Unregistered asset info: {} (ID: {})", it->second.displayName, assetId);
            m_assetInfos.erase(it);
        }
    }

    void AssetManager::UpdateAssetInfo(const AssetInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        m_assetInfos[info.assetId] = info;
        Logger::Debug("Updated asset info: {} (ID: {})", info.displayName, info.assetId);
    }

    const AssetInfo *AssetManager::GetAssetInfo(const std::string &assetId) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        auto it = m_assetInfos.find(assetId);
        return (it != m_assetInfos.end()) ? &it->second : nullptr;
    }

    std::vector<AssetInfo> AssetManager::GetAssetInfosByType(AssetType type) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assetInfos)
        {
            if (pair.second.type == type)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetManager::GetAssetInfosByDirectory(const std::string &directory) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assetInfos)
        {
            if (pair.second.GetDirectory() == directory)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetManager::GetAllAssetInfos() const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assetInfos)
        {
            result.push_back(pair.second);
        }

        return result;
    }

    void AssetManager::ScanResourceDirectories()
    {
        Logger::Debug("Scanning resource directories...");

        // 添加默认资源目录
        if (m_resourceDirectories.empty())
        {
            AddResourceDirectory("Res");
            AddResourceDirectory("Assets");
        }

        for (const auto &dir : m_resourceDirectories)
        {
            ScanDirectory(dir, true);
        }

        Logger::Debug("Resource scan completed. Found {} asset infos.", GetAssetInfoCount());
    }

    void AssetManager::ScanDirectory(const std::string &directory, bool recursive)
    {
        try
        {
            if (!std::filesystem::exists(directory))
            {
                Logger::Warn("Directory does not exist: {}", directory);
                return;
            }

            if (recursive)
            {
                for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))
                {
                    if (entry.is_regular_file())
                    {
                        ProcessFile(entry.path());
                    }
                }
            }
            else
            {
                for (const auto &entry : std::filesystem::directory_iterator(directory))
                {
                    if (entry.is_regular_file())
                    {
                        ProcessFile(entry.path());
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            Logger::Error("Filesystem error while scanning directory {}: {}", directory, e.what());
        }
    }

    void AssetManager::AddResourceDirectory(const std::string &directory)
    {
        auto it = std::find(m_resourceDirectories.begin(), m_resourceDirectories.end(), directory);
        if (it == m_resourceDirectories.end())
        {
            m_resourceDirectories.push_back(directory);
            Logger::Debug("Added resource directory: {}", directory);
        }
    }

    void AssetManager::RemoveResourceDirectory(const std::string &directory)
    {
        auto it = std::find(m_resourceDirectories.begin(), m_resourceDirectories.end(), directory);
        if (it != m_resourceDirectories.end())
        {
            m_resourceDirectories.erase(it);
            Logger::Debug("Removed resource directory: {}", directory);
        }
    }

    std::vector<AssetInfo> AssetManager::SearchAssets(const std::string &query) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto &pair : m_assetInfos)
        {
            std::string lowerName = pair.second.displayName;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

            if (lowerName.find(lowerQuery) != std::string::npos)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetManager::FindAssetsByName(const std::string &name) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assetInfos)
        {
            if (pair.second.displayName == name)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetManager::FindAssetsByExtension(const std::string &extension) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assetInfos)
        {
            if (pair.second.GetFileExtension() == extension)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    void AssetManager::AddDependency(const std::string &assetId, const std::string &dependencyId)
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);

        auto it = m_assetInfos.find(assetId);
        if (it != m_assetInfos.end())
        {
            auto &dependencies = it->second.dependencies;
            if (std::find(dependencies.begin(), dependencies.end(), dependencyId) == dependencies.end())
            {
                dependencies.push_back(dependencyId);
            }
        }

        // 更新反向依赖
        auto depIt = m_assetInfos.find(dependencyId);
        if (depIt != m_assetInfos.end())
        {
            auto &dependents = depIt->second.dependents;
            if (std::find(dependents.begin(), dependents.end(), assetId) == dependents.end())
            {
                dependents.push_back(assetId);
            }
        }
    }

    void AssetManager::RemoveDependency(const std::string &assetId, const std::string &dependencyId)
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);

        auto it = m_assetInfos.find(assetId);
        if (it != m_assetInfos.end())
        {
            auto &dependencies = it->second.dependencies;
            dependencies.erase(std::remove(dependencies.begin(), dependencies.end(), dependencyId), dependencies.end());
        }

        // 更新反向依赖
        auto depIt = m_assetInfos.find(dependencyId);
        if (depIt != m_assetInfos.end())
        {
            auto &dependents = depIt->second.dependents;
            dependents.erase(std::remove(dependents.begin(), dependents.end(), assetId), dependents.end());
        }
    }

    std::vector<std::string> AssetManager::GetDependencies(const std::string &assetId) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);

        auto it = m_assetInfos.find(assetId);
        if (it != m_assetInfos.end())
        {
            return it->second.dependencies;
        }

        return {};
    }

    std::vector<std::string> AssetManager::GetDependents(const std::string &assetId) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);

        auto it = m_assetInfos.find(assetId);
        if (it != m_assetInfos.end())
        {
            return it->second.dependents;
        }

        return {};
    }

    void AssetManager::SetFileWatcher(bool enable)
    {
        m_fileWatcherEnabled = enable;
        Logger::Debug("File watcher {}", enable ? "enabled" : "disabled");
    }

    void AssetManager::SaveAssetDatabase(const std::string &filePath) const
    {
        // TODO: 实现资源数据库序列化
        Logger::Debug("Saving asset database to: {}", filePath);
    }

    void AssetManager::LoadAssetDatabase(const std::string &filePath)
    {
        // TODO: 实现资源数据库反序列化
        Logger::Debug("Loading asset database from: {}", filePath);
    }

    size_t AssetManager::GetAssetInfoCount() const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        return m_assetInfos.size();
    }

    size_t AssetManager::GetAssetInfoCountByType(AssetType type) const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        size_t count = 0;

        for (const auto &pair : m_assetInfos)
        {
            if (pair.second.type == type)
            {
                count++;
            }
        }

        return count;
    }

    size_t AssetManager::GetTotalFileSize() const
    {
        std::lock_guard<std::mutex> lock(m_assetInfoMutex);
        size_t totalSize = 0;

        for (const auto &pair : m_assetInfos)
        {
            totalSize += pair.second.fileSize;
        }

        return totalSize;
    }

    void AssetManager::ProcessFile(const std::filesystem::path &filePath)
    {
        if (!IsAssetFile(filePath.string()))
        {
            return;
        }

        try
        {
            AssetInfo info;
            info.assetId = GenerateAssetId(filePath.string());
            info.displayName = filePath.filename().string();
            info.filePath = filePath.string();
            info.type = DeduceAssetType(filePath.string());
            info.fileSize = std::filesystem::file_size(filePath);

            auto ftime = std::filesystem::last_write_time(filePath);
            #if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
            info.lastModified = std::chrono::system_clock::to_time_t(std::chrono::clock_cast<std::chrono::system_clock>(ftime));
            #else
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - std::filesystem::file_time_type::clock::now()
                + std::chrono::system_clock::now());
            info.lastModified = std::chrono::system_clock::to_time_t(sctp);
            #endif

            RegisterAssetInfo(info);
        }
        catch (const std::exception &e)
        {
            Logger::Error("Error processing file {}: {}", filePath.string(), e.what());
        }
    }

    std::string AssetManager::GenerateAssetId(const std::string &filePath) const
    {
        // 使用文件路径的哈希值作为ID
        std::hash<std::string> hasher;
        size_t hash = hasher(filePath);

        std::ostringstream oss;
        oss << std::hex << hash;
        return oss.str();
    }

    AssetType AssetManager::DeduceAssetType(const std::string &filePath) const
    {
        std::filesystem::path path(filePath);
        std::string extension = path.extension().string();

        // 转换为小写
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        auto it = s_supportedExtensions.find(extension);
        return (it != s_supportedExtensions.end()) ? it->second : AssetType::Unknown;
    }

    bool AssetManager::IsAssetFile(const std::string &filePath) const
    {
        return DeduceAssetType(filePath) != AssetType::Unknown;
    }

    // AssetInfo 方法实现
    std::string AssetInfo::GetFileExtension() const
    {
        std::filesystem::path path(filePath);
        return path.extension().string();
    }

    std::string AssetInfo::GetFileName() const
    {
        std::filesystem::path path(filePath);
        return path.filename().string();
    }

    std::string AssetInfo::GetDirectory() const
    {
        std::filesystem::path path(filePath);
        return path.parent_path().string();
    }

    bool AssetInfo::IsNewer(const AssetInfo &other) const
    {
        return lastModified > other.lastModified;
    }

    rapidjson::Value AssetInfo::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value obj(rapidjson::kObjectType);

        obj.AddMember("assetId", rapidjson::StringRef(assetId.c_str()), allocator);
        obj.AddMember("displayName", rapidjson::StringRef(displayName.c_str()), allocator);
        obj.AddMember("filePath", rapidjson::StringRef(filePath.c_str()), allocator);
        obj.AddMember("type", static_cast<int>(type), allocator);
        obj.AddMember("fileSize", fileSize, allocator);
        obj.AddMember("lastModified", static_cast<int64_t>(lastModified), allocator);

        return obj;
    }

    void AssetInfo::Deserialize(const rapidjson::Value &json)
    {
        if (json.HasMember("assetId") && json["assetId"].IsString())
            assetId = json["assetId"].GetString();

        if (json.HasMember("displayName") && json["displayName"].IsString())
            displayName = json["displayName"].GetString();

        if (json.HasMember("filePath") && json["filePath"].IsString())
            filePath = json["filePath"].GetString();

        if (json.HasMember("type") && json["type"].IsInt())
            type = static_cast<AssetType>(json["type"].GetInt());

        if (json.HasMember("fileSize") && json["fileSize"].IsUint64())
            fileSize = json["fileSize"].GetUint64();

        if (json.HasMember("lastModified") && json["lastModified"].IsInt64())
            lastModified = static_cast<std::time_t>(json["lastModified"].GetInt64());
    }

} // namespace framework
