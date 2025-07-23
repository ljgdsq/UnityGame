#include "Framework/Log/Logger.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Core/Texture.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

namespace framework
{

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

} // namespace framework
