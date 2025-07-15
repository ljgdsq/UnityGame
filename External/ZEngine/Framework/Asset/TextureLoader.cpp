#include "Framework/Asset/TextureLoader.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Util/FileUtil.hpp"
#include <algorithm>
#include <filesystem>

namespace framework
{
    std::shared_ptr<Asset> TextureLoader::LoadAsset(const std::string &assetPath)
    {
        Logger::Debug("Loading texture asset from: {}", assetPath);

        // 检查文件是否存在
        if (!std::filesystem::exists(assetPath))
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

        // 加载纹理数据
        if (!textureAsset->LoadFromFile(assetPath))
        {
            Logger::Error("Failed to load texture from file: {}", assetPath);
            return nullptr;
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
            "png", "jpg", "jpeg", "bmp", "tga", "dds", "hdr", "pic", "pnm", "psd"};
    }

    std::string TextureLoader::GetName() const
    {
        return "TextureLoader";
    }

    bool TextureLoader::LoadImageData(const std::string &filePath, unsigned char *&data, int &width, int &height, int &channels)
    {
        // 这个方法作为后备，实际加载逻辑已经在TextureAsset::LoadFromFile中实现
        // 如果需要直接访问图像数据，可以在这里使用stb_image或其他图像库
        Logger::Debug("LoadImageData called for: {}", filePath);

        // 实际的图像加载逻辑应该在Texture类中实现
        // 这里只是一个接口预留
        return false;
    }

    void TextureLoader::FreeImageData(unsigned char *data)
    {
        // 对应LoadImageData的清理方法
        // 实际的内存管理应该在Texture类中处理
        if (data)
        {
            // 这里应该调用对应的图像库释放函数
            // 例如：stbi_image_free(data);
        }
    }

} // namespace framework
