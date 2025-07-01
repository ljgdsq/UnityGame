#include "Framework/Log/Logger.h"
#include "Framework/Core/ResLoader.h"
#include "Framework/Core/Texture.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace framework {

Texture::Texture() 
    : m_textureID(0), m_width(0), m_height(0), m_channels(0) {
}

Texture::~Texture() {
    Delete();
}

bool Texture::LoadFromFile(const std::string& filepath) {
    // Use ResLoader to resolve the correct path
    auto resLoader = ResLoader::GetInstance();
    auto resolvedPath = resLoader.FindResourcePath("Textures/" + filepath);
    
    if (resolvedPath.empty()) {
        return false;
    }
    
    m_filepath = resolvedPath.string();
    
    // Load image data
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(resolvedPath.string().c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!data) {
        Logger::Error("Failed to load texture: {}", filepath);
        return false;
    }
    
    // Generate texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Determine format based on channels
    GLenum format = GL_RGB;
    if (m_channels == 1) {
        format = GL_RED;
    } else if (m_channels == 3) {
        format = GL_RGB;
    } else if (m_channels == 4) {
        format = GL_RGBA;
    }
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Free image data
    stbi_image_free(data);
    Logger::Log("Loaded texture:{} width:{} height:{} channels:{}",
                m_filepath, m_width, m_height, m_channels);
    return true;
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

std::unique_ptr<Texture> Texture::LoadTexture(const std::string& filename) {
    auto texture = std::make_unique<Texture>();
    if (texture->LoadFromFile(filename)) {
        return texture;
    }
    return nullptr;
}

} // namespace framework
