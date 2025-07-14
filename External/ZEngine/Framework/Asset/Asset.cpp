#include "Framework/Asset/Asset.h"
#include "Framework/Log/Logger.h"
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace framework
{
    Asset::Asset(const std::string &name, AssetType type)
        : m_name(name), m_type(type)
    {
        m_assetId = GenerateAssetId();
    }

    Asset::Asset(const std::string &name, AssetType type, const std::string &assetId)
        : m_name(name), m_type(type), m_assetId(assetId)
    {
        if (m_assetId.empty())
        {
            m_assetId = GenerateAssetId();
        }
    }

    void Asset::Release()
    {
        if (--m_refCount <= 0)
        {
            delete this;
        }
    }

    std::string Asset::GenerateAssetId() const
    {
        // 基于名称、类型和时间戳生成唯一ID
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << AssetTypeToString(m_type) << "_" << m_name << "_" << std::hex << time_t;
        return oss.str();
    }

    rapidjson::Value Asset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);

        // 基本信息
        jsonValue.AddMember("assetId", rapidjson::Value(m_assetId.c_str(), allocator), allocator);
        jsonValue.AddMember("name", rapidjson::Value(m_name.c_str(), allocator), allocator);
        jsonValue.AddMember("type", rapidjson::Value(AssetTypeToString(m_type).c_str(), allocator), allocator);
        jsonValue.AddMember("filePath", rapidjson::Value(m_filePath.c_str(), allocator), allocator);

        // 时间戳
        jsonValue.AddMember("lastModified", static_cast<int64_t>(m_lastModified), allocator);

        // 依赖关系
        if (!m_dependencies.empty())
        {
            rapidjson::Value dependenciesArray(rapidjson::kArrayType);
            for (const auto &dep : m_dependencies)
            {
                dependenciesArray.PushBack(rapidjson::Value(dep.c_str(), allocator), allocator);
            }
            jsonValue.AddMember("dependencies", dependenciesArray, allocator);
        }

        return jsonValue;
    }

    void Asset::Deserialize(const rapidjson::Value &json)
    {
        if (json.HasMember("assetId") && json["assetId"].IsString())
        {
            m_assetId = json["assetId"].GetString();
        }

        if (json.HasMember("name") && json["name"].IsString())
        {
            m_name = json["name"].GetString();
        }

        if (json.HasMember("type") && json["type"].IsString())
        {
            m_type = StringToAssetType(json["type"].GetString());
        }

        if (json.HasMember("filePath") && json["filePath"].IsString())
        {
            m_filePath = json["filePath"].GetString();
        }

        if (json.HasMember("lastModified") && json["lastModified"].IsInt64())
        {
            m_lastModified = json["lastModified"].GetInt64();
        }

        // 加载依赖关系
        if (json.HasMember("dependencies") && json["dependencies"].IsArray())
        {
            m_dependencies.clear();
            const auto &depsArray = json["dependencies"];
            for (rapidjson::SizeType i = 0; i < depsArray.Size(); ++i)
            {
                if (depsArray[i].IsString())
                {
                    m_dependencies.push_back(depsArray[i].GetString());
                }
            }
        }
    }

    void Asset::AddDependency(const std::string &assetId)
    {
        if (std::find(m_dependencies.begin(), m_dependencies.end(), assetId) == m_dependencies.end())
        {
            m_dependencies.push_back(assetId);
        }
    }

    void Asset::RemoveDependency(const std::string &assetId)
    {
        auto it = std::find(m_dependencies.begin(), m_dependencies.end(), assetId);
        if (it != m_dependencies.end())
        {
            m_dependencies.erase(it);
        }
    }

    // 辅助函数实现
    std::string AssetTypeToString(AssetType type)
    {
        switch (type)
        {
        case AssetType::Texture:
            return "Texture";
        case AssetType::Shader:
            return "Shader";
        case AssetType::Mesh:
            return "Mesh";
        case AssetType::Material:
            return "Material";
        case AssetType::Audio:
            return "Audio";
        case AssetType::Animation:
            return "Animation";
        case AssetType::Font:
            return "Font";
        case AssetType::Unknown:
        default:
            return "Unknown";
        }
    }

    AssetType StringToAssetType(const std::string &typeStr)
    {
        if (typeStr == "Texture")
            return AssetType::Texture;
        if (typeStr == "Shader")
            return AssetType::Shader;
        if (typeStr == "Mesh")
            return AssetType::Mesh;
        if (typeStr == "Material")
            return AssetType::Material;
        if (typeStr == "Audio")
            return AssetType::Audio;
        if (typeStr == "Animation")
            return AssetType::Animation;
        if (typeStr == "Font")
            return AssetType::Font;
        return AssetType::Unknown;
    }
}
