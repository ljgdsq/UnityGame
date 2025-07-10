#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Core/ResLoader.h"
#include "Framework/Util/FileUtil.hpp"
namespace framework {
    AssetManager* AssetManager::s_instance = nullptr;


    AssetManager& AssetManager::GetInstance() {
        if (!s_instance) {
            s_instance = new AssetManager();
            Logger::Log("AssetManager instance created");
        }
        return *s_instance;
    }

    AssetManager::~AssetManager() {
        UnloadAllAssets();
        m_assets.clear();
        m_loaders.clear();
        Logger::Log("AssetManager destroyed and all assets unloaded");
    }

    void AssetManager::RegisterLoader(std::shared_ptr<AssetLoader> loader) {
        std::lock_guard<std::mutex> lock(m_loaderMutex);

        // 检查是否已经注册了同名加载器
        auto it = std::find_if(m_loaders.begin(), m_loaders.end(),
            [&loader](const std::shared_ptr<AssetLoader>& existingLoader) {
                return existingLoader->GetName() == loader->GetName();
            });

        if (it != m_loaders.end()) {
            Logger::Warn("Asset loader '{}' is already registered, skipping registration", loader->GetName());
            return;
        }

            m_loaders.push_back(loader);
        Logger::Log("Registered asset loader: {}", loader->GetName());
        auto extensions = loader->GetSupportedExtensions();
        std::string supportedExtensions;
        for (const auto& ext : extensions) {
            if (!supportedExtensions.empty()) {
                supportedExtensions += ", ";
            }
            supportedExtensions += ext;
        }

        Logger::Log("Loader '{}' supports extensions: {}", loader->GetName(), supportedExtensions);
    
    }

    void AssetManager::UnregisterLoader(std::shared_ptr<AssetLoader> loader) {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        auto it = std::remove(m_loaders.begin(), m_loaders.end(), loader);
        if (it != m_loaders.end()) {
            m_loaders.erase(it, m_loaders.end());
            Logger::Log("Unregistered asset loader: {}", loader->GetName());
        } else {
            Logger::Warn("Attempted to unregister non-existent asset loader: {}", loader->GetName());
        }
    }

    template <class T>
    std::shared_ptr<T> AssetManager::LoadAsset(const std::string &assetPath) {
        // 首先尝试从已加载的资源中获取
        std::string assetName = FileUtil::ExtractFileName(assetPath);
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assets.find(assetName);
            if (it != m_assets.end()) {
                auto asset = std::dynamic_pointer_cast<T>(it->second);
                if (asset) {
                    asset->AddRef(); // 增加引用计数
                    Logger::Log("Asset '{}' already loaded, returning existing instance", assetName);
                    return asset;
                } else {
                    Logger::Error("Asset '{}' is not of type {}", assetName, typeid(T).name());
                    return nullptr;
                }
            }
        }

        // 加载新资源
        auto asset = LoadAsset<Asset>(assetPath);
        if (asset)
        {
            return std::dynamic_pointer_cast<T>(asset);
        }
        return nullptr;
    }

    std::shared_ptr<Asset> AssetManager::LoadAsset(const std::string &assetPath, AssetType type) {
        Logger::Debug("Loading asset from path: {}", assetPath);
        if(!ResLoader::GetInstance().FileExists(assetPath)){
            Logger::Error("Asset file does not exist: {}", assetPath);
            return nullptr;
        }

        std::string assetName = FileUtil::ExtractFileName(assetPath);
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_assets.find(assetName);
            if (it != m_assets.end()) {
                Logger::Log("Asset '{}' already loaded, returning existing instance", assetName);
                return it->second; // 返回已加载的资源
            }
        }

        // 查找合适的加载器
        AssetLoader *loader = FindLoader(assetPath);
        if (!loader) {
            Logger::Error("No suitable asset loader found for path: {}", assetPath);
            return nullptr;
        }

        Logger::Debug("Using loader '{}' to load asset: {}", loader->GetName(), assetName);
        std::shared_ptr<Asset> asset = loader->LoadAsset(assetPath);
        if (!asset) {
            Logger::Error("Failed to load asset from path: {}", assetPath);
            return nullptr;
        }

        // check asset 
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_assets[assetName] = asset; // 存储已加载的资源
        }
        Logger::Log("Successfully loaded asset: {}", assetName);
return asset;

    }

    std::shared_ptr<Asset> AssetManager::GetAsset(const std::string &assetName) {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assets.find(assetName);
        if (it != m_assets.end()) {
            return it->second; // 返回已加载的资源
        }
        Logger::Warn("Asset '{}' not found", assetName);
        return nullptr;
    }

    bool AssetManager::HasAsset(const std::string &assetName) const {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        return m_assets.find(assetName) != m_assets.end();
    }

    void AssetManager::UnloadAsset(const std::string &assetName) {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_assets.find(assetName);
        if (it != m_assets.end()) {
            it->second->Unload(); // 调用资源的卸载方法
            it->second->Release(); // 减少引用计数
            m_assets.erase(it); // 从集合中移除
            Logger::Log("Asset '{}' unloaded successfully", assetName);
        } else {
            Logger::Warn("Attempted to unload non-existent asset: {}", assetName);
        }
    }

    void AssetManager::UnloadAllAssets() {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        for (auto &pair : m_assets) {
            pair.second->Unload(); // 调用资源的卸载方法
            pair.second->Release(); // 减少引用计数
        }
        m_assets.clear(); // 清空已加载的资源集合
        Logger::Log("All assets unloaded successfully");
    }

    void AssetManager::PreloadAssets(const std::vector<std::string> &assetPaths) {
        Logger::Error("PreloadAssets is not implemented yet");
    }

    long AssetManager::GetTotalAssetsSize() const {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        long totalSize = 0;
        for (const auto &pair : m_assets) {
            totalSize += pair.second->GetSize(); // 累加每个资源的大小
        }
        return totalSize;
    }

    long AssetManager::GetTotalAssetsCount() const {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        return m_assets.size(); // 返回已加载资源的数量
    }

    std::vector<std::string> AssetManager::GetAllAssetNames() const {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        std::vector<std::string> assetNames;
        for (const auto &pair : m_assets) {
            assetNames.push_back(pair.first); // 收集所有资源的名称
        }
        return assetNames;
    }

    AssetLoader* AssetManager::FindLoader(const std::string &assetPath) const {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        for (const auto &loader : m_loaders) {
            if (loader->CanLoadAsset(assetPath)) {
                return loader.get(); // 返回找到的加载器
            }
        }
        return nullptr; // 没有找到合适的加载器
    }

    AssetLoader* AssetManager::FindLoaderByType(AssetType type) const {
        std::lock_guard<std::mutex> lock(m_loaderMutex);
        for (const auto &loader : m_loaders) {
            if (loader->GetAssetType() == type) {
                return loader.get(); // 返回找到的加载器
            }
        }
        return nullptr; // 没有找到合适的加载器
    }

    void AssetManager::CheckForChanges() {

    }

    

}