#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Core/Texture.h"
#include <memory>

namespace framework
{
    class TextureAsset : public Asset
    {
    public:

        TextureAsset(const std::string &name);
        TextureAsset(unsigned int textureID,
                     const std::string &name,
                     int width = 0,
                     int height = 0,
                     int channels = 0);
        virtual ~TextureAsset() override;

        long GetSize() const override;

        // 序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &json) override;

        // TextureAsset特有方法
        std::shared_ptr<Texture> GetTexture() const { return m_texture; }
        void SetTexture(std::shared_ptr<Texture> texture);

        // 纹理类型
        TextureType GetTextureType() const { return m_textureType; }
        void SetTextureType(TextureType type) { m_textureType = type; }

        // 纹理信息
        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;

        // 纹理设置
        struct TextureSettings
        {
            bool generateMipmaps = true;
            bool sRGB = false;
            int wrapModeS = 0x2901; // GL_REPEAT
            int wrapModeT = 0x2901; // GL_REPEAT
            int minFilter = 0x2703; // GL_LINEAR_MIPMAP_LINEAR
            int magFilter = 0x2601; // GL_LINEAR
            int maxAnisotropy = 16;
        };

        const TextureSettings &GetTextureSettings() const { return m_settings; }
        void SetTextureSettings(const TextureSettings &settings) { m_settings = settings; }

        // 创建空纹理
        void CreateEmpty(int width, int height, int channels = 4);

        // 压缩支持
        bool IsCompressed() const { return m_isCompressed; }
        void SetCompressed(bool compressed) { m_isCompressed = compressed; }

        void GenerateThumbnail() override;

    private:
        std::shared_ptr<Texture> m_texture;
        TextureType m_textureType = TextureType::DIFFUSE;
        TextureSettings m_settings;
        bool m_isCompressed = false;

        // 辅助方法
        void ApplyTextureSettings();
        void GenerateThumbnailFromTexture();
    };

} // namespace framework
