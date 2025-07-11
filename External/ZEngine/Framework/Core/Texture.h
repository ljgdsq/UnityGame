#pragma once
#include <string>

namespace framework
{
    enum class TextureType
    {
        DIFFUSE,           // 漫反射
        NORMAL,            // 法线
        SPECULAR,          // 镜面反射
        METALLIC,          // 金属度
        ROUGHNESS,         // 粗糙度
        AMBIENT_OCCLUSION, // 环境遮挡
        EMISSION,          // 发射
        HEIGHT,            // 高度
        CUSTOM             // 自定义
    };

    class Texture
    {
    public:
        Texture();
        ~Texture();

        // Load texture from file
        bool LoadFromFile(const std::string &filepath);

        // Static factory method that uses ResLoader for path resolution
        static Texture *LoadTexture(const std::string &filename);

        // Bind texture for rendering
        void Bind(unsigned int slot = 0) const;

        // Unbind texture
        void Unbind() const;

        // Get texture properties
        unsigned int GetTextureID() const { return m_textureID; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        int GetChannels() const { return m_channels; }

        // Get the resolved file path
        const std::string &GetFilePath() const { return m_filepath; }

        // Delete texture
        void Delete();

        unsigned int GetId() const;

    private:
        unsigned int m_textureID = 0;
        int m_width;
        int m_height;
        int m_channels;
        std::string m_filepath;
    };

    struct TextureBinding
    {
        std::string name;  // 纹理名称
        TextureType type;  // 纹理类型
        unsigned int slot; // 绑定槽位
        Texture *texture;  // 纹理对象
    };

} // namespace framework
