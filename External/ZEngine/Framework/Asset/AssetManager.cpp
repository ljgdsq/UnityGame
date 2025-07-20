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
namespace framework
{
    AssetManager *AssetManager::s_instance = nullptr;

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

        // 初始化AssetRegistry
        AssetRegistry::GetInstance().ScanResourceDirectories();

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

        // 从AssetRegistry获取资源信息
        const AssetInfo *info = AssetRegistry::GetInstance().GetAssetInfo(assetId);
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
        // 从AssetRegistry获取资源信息
        const AssetInfo *info = AssetRegistry::GetInstance().GetAssetInfo(assetId);
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

} // namespace framework
