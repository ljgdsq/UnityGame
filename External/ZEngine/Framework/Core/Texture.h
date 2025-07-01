#pragma once
#include <string>
#include <memory>

namespace framework {

class Texture {
public:
    Texture();
    ~Texture();

    // Load texture from file
    bool LoadFromFile(const std::string& filepath);
    
    // Static factory method that uses ResLoader for path resolution
    static std::unique_ptr<Texture> LoadTexture(const std::string& filename);
    
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
    const std::string& GetFilePath() const { return m_filepath; }

    // Delete texture
    void Delete();

private:
    unsigned int m_textureID;
    int m_width;
    int m_height;
    int m_channels;
    std::string m_filepath;
};

} // namespace framework
