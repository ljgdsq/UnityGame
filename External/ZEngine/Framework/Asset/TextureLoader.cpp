#include "Framework/Asset/TextureLoader.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Util/FileUtil.hpp"
#include <algorithm>
#include "GLFW/glfw3.h"
#include "Framework/Asset/AssetRegister.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace framework
{
    REGISTER_LOADER(TextureLoader);

    std::shared_ptr<Asset> TextureLoader::LoadAsset(const std::string &assetPath)
    {
        Logger::Debug("Loading texture asset from: {}", assetPath);

        // 检查文件是否存在
        if (!EngineFileIO::FileExists(assetPath))
        {
            Logger::Error("Texture file does not exist: {}", assetPath);
            return nullptr;
        }

        // 获取文件名作为资源名称
        std::string assetName = FileUtil::ExtractFileName(assetPath);

        // 创建TextureAsset
        auto textureAsset = std::make_shared<TextureAsset>(assetName);

        // 设置文件路径
        textureAsset->SetFilePath(assetPath);
        textureAsset->SetLoadState(LoadState::Loading);

        auto data = EngineFileIO::LoadBinary(assetPath);

        // 加载纹理数据
        if (!GenTexture(data, textureAsset))
        {
            textureAsset->SetLoadState(LoadState::Failed);
            Logger::Error("Failed to load texture from file: {}", assetPath);
            return textureAsset;
        }

        // 设置加载状态
        textureAsset->SetLoadState(LoadState::Loaded);

        Logger::Debug("Successfully loaded texture asset: {}", assetName);
        return textureAsset;
    }

    bool TextureLoader::CanLoadAsset(const std::string &assetPath) const
    {
        std::string extension = FileUtil::GetFileExtension(assetPath);

        // 转换为小写进行比较
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        auto supportedExtensions = GetSupportedExtensions();
        return std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end();
    }

    std::vector<std::string> TextureLoader::GetSupportedExtensions() const
    {
        return {
            "png", "jpg", "jpeg", "bmp"};
    }

    std::string TextureLoader::GetName() const
    {
        return "TextureLoader";
    }

    bool TextureLoader::GenTexture(const std::vector<uint8_t> &data, std::shared_ptr<TextureAsset> asset)
    {
        int w, h, n;
        unsigned int id;
        void *pixels = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &w, &h, &n, 0);
        if (!pixels)
        {
            Logger::Error("Failed to load texture from memory: {}", stbi_failure_reason());
            return false;
        }
        Logger::Debug("Loaded texture: {}x{} with {} channels", w, h, n);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        if (n == 1)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
        }
        else if (n == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        }
        else if (n == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }
        else
        {
            Logger::Error("Unsupported number of channels: {}", n);
            stbi_image_free(pixels);
            glDeleteTextures(1, &id);
            return false;
        }

        auto texture = std::make_shared<Texture>(asset->GetFilePath(), id, w, h, n);
        asset->SetTexture(texture);

        // 释放像素数据
        stbi_image_free(pixels);

        return true;
    }

} // namespace framework
