#include "Framework/Asset/TextureAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/ResLoader.h"
#include "glad/glad.h"
#include <algorithm>
#include <filesystem>

namespace framework
{
    TextureAsset::TextureAsset(const std::string &name)
        : Asset(name, AssetType::Texture)
    {
        Logger::Debug("Created TextureAsset: {}", name);
    }

    TextureAsset::TextureAsset(const std::string &name, const std::string &assetId)
        : Asset(name, AssetType::Texture, assetId)
    {
        Logger::Debug("Created TextureAsset: {} (ID: {})", name, assetId);
    }

    TextureAsset::~TextureAsset()
    {
        Unload();
        ReleaseThumbnail();
        Logger::Debug("Destroyed TextureAsset: {}", GetName());
    }

    void TextureAsset::Load()
    {
        if (IsLoaded())
        {
            return;
        }

        SetLoadState(LoadState::Loading);

        try
        {
            if (GetFilePath().empty())
            {
                Logger::Error("TextureAsset::Load - No file path set for asset: {}", GetName());
                SetLoadState(LoadState::Failed);
                return;
            }

            if (LoadFromFile(GetFilePath()))
            {
                SetLoadState(LoadState::Loaded);
                Logger::Debug("Successfully loaded texture: {}", GetName());
            }
            else
            {
                SetLoadState(LoadState::Failed);
                Logger::Error("Failed to load texture: {}", GetName());
            }
        }
        catch (const std::exception &e)
        {
            SetLoadState(LoadState::Failed);
            Logger::Error("Exception while loading texture {}: {}", GetName(), e.what());
        }
    }

    void TextureAsset::Unload()
    {
        if (!IsLoaded())
        {
            return;
        }

        if (m_texture)
        {
            m_texture->Delete();
            m_texture.reset();
        }

        ReleaseThumbnail();
        SetLoadState(LoadState::NotLoaded);
        Logger::Debug("Unloaded texture: {}", GetName());
    }

    long TextureAsset::GetSize() const
    {
        if (!m_texture)
        {
            return 0;
        }

        // 估算纹理内存大小
        long size = GetWidth() * GetHeight() * GetChannels();

        // 如果有mipmap，增加约33%的大小
        if (m_settings.generateMipmaps)
        {
            size = size * 4 / 3;
        }

        return size;
    }

    rapidjson::Value TextureAsset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue = Asset::Serialize(allocator);

        // 添加TextureAsset特有的属性
        jsonValue.AddMember("textureType", static_cast<int>(m_textureType), allocator);
        jsonValue.AddMember("isCompressed", m_isCompressed, allocator);

        // 序列化纹理设置
        rapidjson::Value settingsValue(rapidjson::kObjectType);
        settingsValue.AddMember("generateMipmaps", m_settings.generateMipmaps, allocator);
        settingsValue.AddMember("sRGB", m_settings.sRGB, allocator);
        settingsValue.AddMember("wrapModeS", m_settings.wrapModeS, allocator);
        settingsValue.AddMember("wrapModeT", m_settings.wrapModeT, allocator);
        settingsValue.AddMember("minFilter", m_settings.minFilter, allocator);
        settingsValue.AddMember("magFilter", m_settings.magFilter, allocator);
        settingsValue.AddMember("maxAnisotropy", m_settings.maxAnisotropy, allocator);

        jsonValue.AddMember("settings", settingsValue, allocator);

        return jsonValue;
    }

    void TextureAsset::Deserialize(const rapidjson::Value &json)
    {
        Asset::Deserialize(json);

        if (json.HasMember("textureType") && json["textureType"].IsInt())
        {
            m_textureType = static_cast<TextureType>(json["textureType"].GetInt());
        }

        if (json.HasMember("isCompressed") && json["isCompressed"].IsBool())
        {
            m_isCompressed = json["isCompressed"].GetBool();
        }

        // 反序列化纹理设置
        if (json.HasMember("settings") && json["settings"].IsObject())
        {
            const auto &settings = json["settings"];

            if (settings.HasMember("generateMipmaps") && settings["generateMipmaps"].IsBool())
                m_settings.generateMipmaps = settings["generateMipmaps"].GetBool();

            if (settings.HasMember("sRGB") && settings["sRGB"].IsBool())
                m_settings.sRGB = settings["sRGB"].GetBool();

            if (settings.HasMember("wrapModeS") && settings["wrapModeS"].IsInt())
                m_settings.wrapModeS = settings["wrapModeS"].GetInt();

            if (settings.HasMember("wrapModeT") && settings["wrapModeT"].IsInt())
                m_settings.wrapModeT = settings["wrapModeT"].GetInt();

            if (settings.HasMember("minFilter") && settings["minFilter"].IsInt())
                m_settings.minFilter = settings["minFilter"].GetInt();

            if (settings.HasMember("magFilter") && settings["magFilter"].IsInt())
                m_settings.magFilter = settings["magFilter"].GetInt();

            if (settings.HasMember("maxAnisotropy") && settings["maxAnisotropy"].IsInt())
                m_settings.maxAnisotropy = settings["maxAnisotropy"].GetInt();
        }
    }

    void TextureAsset::SetTexture(std::shared_ptr<Texture> texture)
    {
        m_texture = texture;
        if (texture)
        {
            ApplyTextureSettings();
            GenerateThumbnailFromTexture();
        }
    }

    int TextureAsset::GetWidth() const
    {
        return m_texture ? m_texture->GetWidth() : 0;
    }

    int TextureAsset::GetHeight() const
    {
        return m_texture ? m_texture->GetHeight() : 0;
    }

    int TextureAsset::GetChannels() const
    {
        return m_texture ? m_texture->GetChannels() : 0;
    }

    bool TextureAsset::LoadFromFile(const std::string &filePath)
    {
        // 初始化ResLoader
        ResLoader::GetInstance().Initialize("Res/");

        // 解析文件路径
        std::string resolvedPath;

        // 尝试不同的路径解析方法
        if (std::filesystem::exists(filePath))
        {
            // 如果是绝对路径且文件存在，直接使用
            resolvedPath = filePath;
        }
        else
        {
            // 尝试通过ResLoader查找文件
            resolvedPath = ResLoader::GetInstance().GetResourcePath(filePath);
            if (resolvedPath.empty() || !std::filesystem::exists(resolvedPath))
            {
                // 尝试纹理专用路径
                resolvedPath = ResLoader::GetInstance().GetTexturePath(filePath);
            }
        }

        if (resolvedPath.empty() || !std::filesystem::exists(resolvedPath))
        {
            Logger::Error("Could not resolve texture path: {}", filePath);
            return false;
        }

        // 创建Texture对象
        m_texture = std::make_shared<Texture>();
        if (!m_texture->LoadFromFile(resolvedPath))
        {
            Logger::Error("Failed to load texture from file: {}", resolvedPath);
            m_texture.reset();
            return false;
        }

        // 应用纹理设置
        ApplyTextureSettings();

        // 生成缩略图
        GenerateThumbnailFromTexture();

        Logger::Debug("Loaded texture: {} ({}x{}x{})", GetName(), GetWidth(), GetHeight(), GetChannels());
        return true;
    }

    void TextureAsset::CreateEmpty(int width, int height, int channels)
    {
        m_texture = std::make_shared<Texture>();

        // 创建空纹理数据
        std::vector<unsigned char> data(width * height * channels, 255);

        // 绑定纹理并上传数据
        m_texture->Bind();

        GLenum format = GL_RGBA;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());

        ApplyTextureSettings();
        GenerateThumbnailFromTexture();

        Logger::Debug("Created empty texture: {} ({}x{}x{})", GetName(), width, height, channels);
    }

    void TextureAsset::ApplyTextureSettings()
    {
        if (!m_texture)
        {
            return;
        }

        m_texture->Bind();

        // 设置包装模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settings.wrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settings.wrapModeT);

        // 设置过滤模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_settings.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_settings.magFilter);

        // 设置各向异性过滤
        // if (m_settings.maxAnisotropy > 1)
        // {
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_settings.maxAnisotropy);
        // }

        // 生成mipmap
        if (m_settings.generateMipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        m_texture->Unbind();
    }

    void TextureAsset::GenerateThumbnail()
    {
        if (m_texture)
        {
            GenerateThumbnailFromTexture();
        }
    }

    void *TextureAsset::GetThumbnailTextureId() const
    {
        return m_thumbnailTextureId;
    }

    bool TextureAsset::HasThumbnail() const
    {
        return m_thumbnailTextureId != nullptr;
    }

    void TextureAsset::GenerateThumbnailFromTexture()
    {
        if (!m_texture)
        {
            Logger::Debug("TextureAsset::GenerateThumbnailFromTexture - No texture available for {}", GetName());
            return;
        }

        unsigned int textureID = m_texture->GetTextureID();
        if (textureID == 0)
        {
            Logger::Debug("TextureAsset::GenerateThumbnailFromTexture - Invalid texture ID for {}", GetName());
            return;
        }

        // 简单实现：直接使用原纹理作为缩略图
        // 在实际项目中，应该生成缩小的版本
        m_thumbnailTextureId = (void *)(intptr_t)textureID;

        Logger::Debug("TextureAsset::GenerateThumbnailFromTexture - Generated thumbnail for {} (ID: {})", GetName(), textureID);

        // TODO: 实现真正的缩略图生成逻辑
        // 1. 创建帧缓冲区
        // 2. 将原纹理渲染到较小的纹理上
        // 3. 保存缩略图纹理ID
    }

    void TextureAsset::ReleaseThumbnail()
    {
        if (m_thumbnailTextureId)
        {
            // 如果缩略图是独立的纹理，需要删除它
            // 当前实现中，缩略图就是原纹理，所以不需要删除
            m_thumbnailTextureId = nullptr;
        }
    }

} // namespace framework
