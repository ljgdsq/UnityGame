#include "Framework/Log/Logger.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Core/Texture.h"
#include "glad/glad.h"

namespace framework
{

    Texture *Texture::s_defaultMissingTexture = nullptr; // 初始化默认缺失纹理为nullptr

    Texture::Texture()
        : m_textureID(0), m_width(0), m_height(0), m_channels(0), m_filepath("")
    {
    }

    Texture::Texture(const std::string &filepath, unsigned int textureID, int width, int height, int channels)
        : m_filepath(filepath), m_textureID(textureID), m_width(width), m_height(height), m_channels(channels)
    {
        Logger::Debug("Created Texture: {} (ID: {}, Size: {}x{}, Channels: {})", filepath, textureID, width, height, channels);
    }

    Texture::~Texture()
    {
        Delete();
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Delete()
    {
        if (m_textureID != 0)
        {
            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        }
    }

    unsigned int Texture::GetId() const
    {
        return m_textureID;
    }

    Texture *Texture::GetDefaultMissingTexture()
    {
        if (s_defaultMissingTexture == nullptr)
        {
            // 直接构造一个32*32的粉红色纹理
            unsigned char pinkData[32 * 32 * 3];
            for (int i = 0; i < 32 * 32 * 3; i += 3)
            {
                pinkData[i] = 255;     // R
                pinkData[i + 1] = 105; // G
                pinkData[i + 2] = 180; // B
            }
            s_defaultMissingTexture = new Texture("DefaultMissingTexture", 0, 32, 32, 3);
            s_defaultMissingTexture->m_textureID = 0; // 这里应该是实际的OpenGL纹理ID
            glGenTextures(1, &s_defaultMissingTexture->m_textureID);
            glBindTexture(GL_TEXTURE_2D, s_defaultMissingTexture->m_textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, pinkData);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        return s_defaultMissingTexture;
    }

} // namespace framework
