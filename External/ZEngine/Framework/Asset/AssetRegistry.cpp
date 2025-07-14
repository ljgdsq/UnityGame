#include "Framework/Asset/AssetRegistry.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/ResLoader.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

namespace framework
{
    // 支持的文件扩展名映射
    const std::unordered_map<std::string, AssetType> AssetRegistry::s_supportedExtensions = {
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

    AssetRegistry &AssetRegistry::GetInstance()
    {
        static AssetRegistry instance;
        return instance;
    }

    void AssetRegistry::RegisterAsset(const AssetInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_assets[info.assetId] = info;
        Logger::Debug("Registered asset: {} (ID: {})", info.displayName, info.assetId);
    }

    void AssetRegistry::UnregisterAsset(const std::string &assetId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_assets.find(assetId);
        if (it != m_assets.end())
        {
            Logger::Debug("Unregistered asset: {} (ID: {})", it->second.displayName, assetId);
            m_assets.erase(it);
        }
    }

    void AssetRegistry::UpdateAsset(const AssetInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_assets[info.assetId] = info;
        Logger::Debug("Updated asset: {} (ID: {})", info.displayName, info.assetId);
    }

    const AssetInfo *AssetRegistry::GetAssetInfo(const std::string &assetId) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_assets.find(assetId);
        return (it != m_assets.end()) ? &it->second : nullptr;
    }

    std::vector<AssetInfo> AssetRegistry::GetAssetsByType(AssetType type) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assets)
        {
            if (pair.second.type == type)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetRegistry::GetAssetsByDirectory(const std::string &directory) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assets)
        {
            if (pair.second.GetDirectory() == directory)
            {
                result.push_back(pair.second);
            }
        }

        return result;
    }

    std::vector<AssetInfo> AssetRegistry::GetAllAssets() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<AssetInfo> result;

        for (const auto &pair : m_assets)
        {
            result.push_back(pair.second);
        }

        return result;
    }

    void AssetRegistry::ScanResourceDirectories()
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

        Logger::Debug("Resource scan completed. Found {} assets.", GetAssetCount());
    }

    void AssetRegistry::ScanDirectory(const std::string &directory, bool recursive)
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

    void AssetRegistry::AddResourceDirectory(const std::string &directory)
    {
        auto it = std::find(m_resourceDirectories.begin(), m_resourceDirectories.end(), directory);
        if (it == m_resourceDirectories.end())
        {
            m_resourceDirectories.push_back(directory);
            Logger::Debug("Added resource directory: {}", directory);
        }
    }

    void AssetRegistry::RemoveResourceDirectory(const std::string &directory)
    {
        auto it = std::find(m_resourceDirectories.begin(), m_resourceDirectories.end(), directory);
        if (it != m_resourceDirectories.end())
        {
            m_resourceDirectories.erase(it);
            Logger::Debug("Removed resource directory: {}", directory);
        }
    }

    std::vector<AssetInfo> AssetRegistry::SearchAssets(const std::string &query) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<AssetInfo> result;

        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto &pair : m_assets)
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

    void AssetRegistry::ProcessFile(const std::filesystem::path &filePath)
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

            RegisterAsset(info);
        }
        catch (const std::exception &e)
        {
            Logger::Error("Error processing file {}: {}", filePath.string(), e.what());
        }
    }

    std::string AssetRegistry::GenerateAssetId(const std::string &filePath) const
    {
        // 使用文件路径的哈希值作为ID
        std::hash<std::string> hasher;
        size_t hash = hasher(filePath);

        std::ostringstream oss;
        oss << std::hex << hash;
        return oss.str();
    }

    AssetType AssetRegistry::DeduceAssetType(const std::string &filePath) const
    {
        std::filesystem::path path(filePath);
        std::string extension = path.extension().string();

        // 转换为小写
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        auto it = s_supportedExtensions.find(extension);
        return (it != s_supportedExtensions.end()) ? it->second : AssetType::Unknown;
    }

    bool AssetRegistry::IsAssetFile(const std::string &filePath) const
    {
        return DeduceAssetType(filePath) != AssetType::Unknown;
    }

    size_t AssetRegistry::GetAssetCount() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_assets.size();
    }

    size_t AssetRegistry::GetAssetCountByType(AssetType type) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        size_t count = 0;

        for (const auto &pair : m_assets)
        {
            if (pair.second.type == type)
            {
                count++;
            }
        }

        return count;
    }

    void AssetRegistry::Clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_assets.clear();
        Logger::Debug("Asset registry cleared.");
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

    // 全局便利函数
    AssetRegistry &GetAssetRegistry()
    {
        return AssetRegistry::GetInstance();
    }

} // namespace framework
