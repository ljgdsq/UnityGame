#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/ResLoader.h"
#include "Framework/Asset/AssetManager.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <filesystem>
#include <fstream>

namespace framework
{
    MaterialAsset::MaterialAsset(const std::string &name)
        : Asset(name, AssetType::Material)
    {
        Logger::Debug("Created MaterialAsset: {}", name);
    }

    MaterialAsset::MaterialAsset(const std::string &name, const std::string &assetId)
        : Asset(name, AssetType::Material, assetId)
    {
        Logger::Debug("Created MaterialAsset: {} (ID: {})", name, assetId);
    }

    MaterialAsset::~MaterialAsset()
    {
        Unload();
        Logger::Debug("Destroyed MaterialAsset: {}", GetName());
    }

    void MaterialAsset::Load()
    {
        if (IsLoaded())
        {
            return;
        }

        SetLoadState(LoadState::Loading);

        try
        {
            // 如果有文件路径，从文件加载
            if (!GetFilePath().empty())
            {
                if (LoadFromFile(GetFilePath()))
                {
                    SetLoadState(LoadState::Loaded);
                    Logger::Debug("Successfully loaded material from file: {}", GetName());
                }
                else
                {
                    SetLoadState(LoadState::Failed);
                    Logger::Error("Failed to load material from file: {}", GetName());
                }
            }
            else
            {
                // 创建默认材质
                CreateDefault();
                SetLoadState(LoadState::Loaded);
                Logger::Debug("Created default material: {}", GetName());
            }
        }
        catch (const std::exception &e)
        {
            SetLoadState(LoadState::Failed);
            Logger::Error("Exception while loading material {}: {}", GetName(), e.what());
        }
    }

    void MaterialAsset::Unload()
    {
        if (!IsLoaded())
        {
            return;
        }

        m_material.reset();
        m_textureReferences.clear();
        m_floatProperties.clear();
        m_intProperties.clear();
        m_vectorProperties.clear();
        m_matrixProperties.clear();

        SetLoadState(LoadState::NotLoaded);
        Logger::Debug("Unloaded material: {}", GetName());
    }

    long MaterialAsset::GetSize() const
    {
        if (!m_material)
        {
            return 0;
        }

        // 估算材质内存大小
        long size = sizeof(Material);

        // 添加属性的大小
        size += m_floatProperties.size() * sizeof(float);
        size += m_intProperties.size() * sizeof(int);
        size += m_vectorProperties.size() * sizeof(glm::vec3);
        size += m_matrixProperties.size() * sizeof(glm::mat4);

        // 添加纹理引用的大小
        size += m_textureReferences.size() * sizeof(AssetReference<TextureAsset>);

        return size;
    }

    void MaterialAsset::GenerateThumbnail()
    {
        if (!m_material)
        {
            return;
        }

        // 生成材质缩略图
        // 这里可以实现材质的预览渲染
        // 暂时使用简单的占位实现
        SetThumbnailTextureId(nullptr);
        Logger::Debug("Generated thumbnail for material: {}", GetName());
    }

    rapidjson::Value MaterialAsset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue = Asset::Serialize(allocator);

        // 序列化着色器名称
        if (!m_shaderName.empty())
        {
            rapidjson::Value shaderValue(m_shaderName.c_str(), allocator);
            jsonValue.AddMember("shader", shaderValue, allocator);
        }

        // 序列化纹理引用
        if (!m_textureReferences.empty())
        {
            rapidjson::Value texturesValue(rapidjson::kObjectType);
            for (const auto &pair : m_textureReferences)
            {
                rapidjson::Value nameValue(pair.first.c_str(), allocator);
                rapidjson::Value refValue = pair.second.Serialize(allocator);
                texturesValue.AddMember(nameValue, refValue, allocator);
            }
            jsonValue.AddMember("textures", texturesValue, allocator);
        }

        // 序列化浮点属性
        if (!m_floatProperties.empty())
        {
            rapidjson::Value floatPropsValue(rapidjson::kObjectType);
            for (const auto &pair : m_floatProperties)
            {
                rapidjson::Value nameValue(pair.first.c_str(), allocator);
                floatPropsValue.AddMember(nameValue, pair.second, allocator);
            }
            jsonValue.AddMember("floatProperties", floatPropsValue, allocator);
        }

        // 序列化整数属性
        if (!m_intProperties.empty())
        {
            rapidjson::Value intPropsValue(rapidjson::kObjectType);
            for (const auto &pair : m_intProperties)
            {
                rapidjson::Value nameValue(pair.first.c_str(), allocator);
                intPropsValue.AddMember(nameValue, pair.second, allocator);
            }
            jsonValue.AddMember("intProperties", intPropsValue, allocator);
        }

        // 序列化向量属性
        if (!m_vectorProperties.empty())
        {
            rapidjson::Value vectorPropsValue(rapidjson::kObjectType);
            for (const auto &pair : m_vectorProperties)
            {
                rapidjson::Value nameValue(pair.first.c_str(), allocator);
                rapidjson::Value vecValue(rapidjson::kArrayType);
                vecValue.PushBack(pair.second.x, allocator);
                vecValue.PushBack(pair.second.y, allocator);
                vecValue.PushBack(pair.second.z, allocator);
                vectorPropsValue.AddMember(nameValue, vecValue, allocator);
            }
            jsonValue.AddMember("vectorProperties", vectorPropsValue, allocator);
        }

        return jsonValue;
    }

    void MaterialAsset::Deserialize(const rapidjson::Value &json)
    {
        Asset::Deserialize(json);

        // 反序列化着色器名称
        if (json.HasMember("shader") && json["shader"].IsString())
        {
            m_shaderName = json["shader"].GetString();
        }

        // 反序列化纹理引用
        if (json.HasMember("textures") && json["textures"].IsObject())
        {
            const auto &textures = json["textures"];
            for (auto it = textures.MemberBegin(); it != textures.MemberEnd(); ++it)
            {
                std::string textureName = it->name.GetString();
                AssetReference<TextureAsset> textureRef;
                textureRef.Deserialize(it->value);
                m_textureReferences[textureName] = textureRef;
            }
        }

        // 反序列化浮点属性
        if (json.HasMember("floatProperties") && json["floatProperties"].IsObject())
        {
            const auto &floatProps = json["floatProperties"];
            for (auto it = floatProps.MemberBegin(); it != floatProps.MemberEnd(); ++it)
            {
                std::string propName = it->name.GetString();
                if (it->value.IsNumber())
                {
                    m_floatProperties[propName] = static_cast<float>(it->value.GetDouble());
                }
            }
        }

        // 反序列化整数属性
        if (json.HasMember("intProperties") && json["intProperties"].IsObject())
        {
            const auto &intProps = json["intProperties"];
            for (auto it = intProps.MemberBegin(); it != intProps.MemberEnd(); ++it)
            {
                std::string propName = it->name.GetString();
                if (it->value.IsInt())
                {
                    m_intProperties[propName] = it->value.GetInt();
                }
            }
        }

        // 反序列化向量属性
        if (json.HasMember("vectorProperties") && json["vectorProperties"].IsObject())
        {
            const auto &vectorProps = json["vectorProperties"];
            for (auto it = vectorProps.MemberBegin(); it != vectorProps.MemberEnd(); ++it)
            {
                std::string propName = it->name.GetString();
                if (it->value.IsArray() && it->value.Size() >= 3)
                {
                    glm::vec3 vec(
                        static_cast<float>(it->value[0].GetDouble()),
                        static_cast<float>(it->value[1].GetDouble()),
                        static_cast<float>(it->value[2].GetDouble()));
                    m_vectorProperties[propName] = vec;
                }
            }
        }
    }

    std::shared_ptr<Material> MaterialAsset::GetMaterial() const
    {
        return m_material;
    }

    void MaterialAsset::SetMaterial(std::shared_ptr<Material> material)
    {
        m_material = material;
        if (material)
        {
            // 将属性应用到材质
            ApplyPropertiesToMaterial();
            ApplyTexturesToMaterial();
        }
    }

    void MaterialAsset::SetShader(const std::string &shaderName)
    {
        m_shaderName = shaderName;
        if (m_material)
        {
            m_material->SetShader(shaderName);
        }
    }

    std::string MaterialAsset::GetShaderName() const
    {
        return m_shaderName;
    }

    void MaterialAsset::SetTexture(const std::string &name, AssetReference<TextureAsset> textureRef, int slot)
    {
        m_textureReferences[name] = textureRef;
        UpdateDependencies();

        if (m_material)
        {
            ApplyTexturesToMaterial();
        }
    }

    void MaterialAsset::SetTexture(const std::string &name, const std::string &textureAssetId, int slot)
    {
        AssetReference<TextureAsset> textureRef(textureAssetId);
        SetTexture(name, textureRef, slot);
    }

    AssetReference<TextureAsset> MaterialAsset::GetTextureReference(const std::string &name) const
    {
        auto it = m_textureReferences.find(name);
        if (it != m_textureReferences.end())
        {
            return it->second;
        }
        return AssetReference<TextureAsset>();
    }

    void MaterialAsset::SetFloat(const std::string &name, float value)
    {
        m_floatProperties[name] = value;
        if (m_material)
        {
            m_material->SetFloat(name, value);
        }
    }

    void MaterialAsset::SetInt(const std::string &name, int value)
    {
        m_intProperties[name] = value;
        if (m_material)
        {
            m_material->SetInt(name, value);
        }
    }

    void MaterialAsset::SetVector(const std::string &name, const glm::vec3 &value)
    {
        m_vectorProperties[name] = value;
        if (m_material)
        {
            m_material->SetVector(name, value);
        }
    }

    void MaterialAsset::SetMatrix(const std::string &name, const glm::mat4 &value)
    {
        m_matrixProperties[name] = value;
        if (m_material)
        {
            m_material->SetMatrix(name, value);
        }
    }

    float MaterialAsset::GetFloat(const std::string &name) const
    {
        auto it = m_floatProperties.find(name);
        return it != m_floatProperties.end() ? it->second : 0.0f;
    }

    int MaterialAsset::GetInt(const std::string &name) const
    {
        auto it = m_intProperties.find(name);
        return it != m_intProperties.end() ? it->second : 0;
    }

    glm::vec3 MaterialAsset::GetVector(const std::string &name) const
    {
        auto it = m_vectorProperties.find(name);
        return it != m_vectorProperties.end() ? it->second : glm::vec3(0.0f);
    }

    glm::mat4 MaterialAsset::GetMatrix(const std::string &name) const
    {
        auto it = m_matrixProperties.find(name);
        return it != m_matrixProperties.end() ? it->second : glm::mat4(1.0f);
    }

    bool MaterialAsset::LoadFromFile(const std::string &filePath)
    {
        // 初始化ResLoader
        ResLoader::GetInstance().Initialize("Res/");

        // 解析文件路径
        std::string resolvedPath;
        if (std::filesystem::exists(filePath))
        {
            resolvedPath = filePath;
        }
        else
        {
            resolvedPath = ResLoader::GetInstance().GetResourcePath(filePath);
        }

        if (resolvedPath.empty() || !std::filesystem::exists(resolvedPath))
        {
            Logger::Error("Could not resolve material path: {}", filePath);
            return false;
        }

        // 读取JSON文件
        std::ifstream file(resolvedPath);
        if (!file.is_open())
        {
            Logger::Error("Failed to open material file: {}", resolvedPath);
            return false;
        }

        std::string jsonContent((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

        rapidjson::Document doc;
        doc.Parse(jsonContent.c_str());

        if (doc.HasParseError())
        {
            Logger::Error("Failed to parse material JSON: {}", resolvedPath);
            return false;
        }

        // 反序列化材质数据
        Deserialize(doc);

        // 创建Material对象
        m_material = std::make_shared<Material>(GetName());

        // 应用属性到材质
        UpdateMaterialFromProperties();

        Logger::Debug("Loaded material from file: {}", resolvedPath);
        return true;
    }

    bool MaterialAsset::SaveToFile(const std::string &filePath) const
    {
        rapidjson::Document doc;
        doc.SetObject();

        auto jsonValue = Serialize(doc.GetAllocator());
        doc.CopyFrom(jsonValue, doc.GetAllocator());

        // 写入文件
        std::ofstream file(filePath);
        if (!file.is_open())
        {
            Logger::Error("Failed to open material file for writing: {}", filePath);
            return false;
        }

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        file << buffer.GetString();
        file.close();

        Logger::Debug("Saved material to file: {}", filePath);
        return true;
    }

    void MaterialAsset::CreateDefault()
    {
        m_material = std::make_shared<Material>(GetName());
        m_shaderName = "DefaultShader";

        // 设置默认属性
        SetFloat("_Metallic", 0.0f);
        SetFloat("_Roughness", 0.5f);
        SetVector("_Color", glm::vec3(1.0f, 1.0f, 1.0f));

        Logger::Debug("Created default material: {}", GetName());
    }

    void MaterialAsset::ApplyPropertiesToMaterial()
    {
        if (!m_material)
        {
            return;
        }

        // 应用浮点属性
        for (const auto &pair : m_floatProperties)
        {
            m_material->SetFloat(pair.first, pair.second);
        }

        // 应用整数属性
        for (const auto &pair : m_intProperties)
        {
            m_material->SetInt(pair.first, pair.second);
        }

        // 应用向量属性
        for (const auto &pair : m_vectorProperties)
        {
            m_material->SetVector(pair.first, pair.second);
        }

        // 应用矩阵属性
        for (const auto &pair : m_matrixProperties)
        {
            m_material->SetMatrix(pair.first, pair.second);
        }
    }

    void MaterialAsset::ApplyTexturesToMaterial()
    {
        if (!m_material)
        {
            return;
        }

        // 应用纹理引用
        for (const auto &pair : m_textureReferences)
        {
            auto textureAsset = pair.second.Get();
            if (textureAsset && textureAsset->IsLoaded())
            {
                auto texture = textureAsset->GetTexture();
                if (texture)
                {
                    // 这里需要根据实际的Material接口进行调整
                    // m_material->SetTexture(pair.first, texture.get());
                }
            }
        }
    }

    void MaterialAsset::UpdateMaterialFromProperties()
    {
        if (!m_material)
        {
            return;
        }

        // 设置着色器
        if (!m_shaderName.empty())
        {
            m_material->SetShader(m_shaderName);
        }

        // 应用所有属性
        ApplyPropertiesToMaterial();
        ApplyTexturesToMaterial();
    }

    void MaterialAsset::UpdateDependencies()
    {
        ClearDependencies();

        // 添加纹理依赖
        for (const auto &pair : m_textureReferences)
        {
            if (!pair.second.GetAssetId().empty())
            {
                AddDependency(pair.second.GetAssetId());
            }
        }
    }

    void MaterialAsset::OnTextureChanged(const std::string &textureName)
    {
        // 纹理变化时的回调
        if (m_material)
        {
            ApplyTexturesToMaterial();
        }
    }

} // namespace framework
