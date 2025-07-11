#include "Framework/Render/Material.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Shader.h"
#include "Material.h"
namespace framework
{
    // 纹理类型辅助函数实现
    const char *TextureTypeToString(TextureType type)
    {
        switch (type)
        {
        case TextureType::DIFFUSE:
            return "Diffuse";
        case TextureType::NORMAL:
            return "Normal";
        case TextureType::SPECULAR:
            return "Specular";
        case TextureType::METALLIC:
            return "Metallic";
        case TextureType::ROUGHNESS:
            return "Roughness";
        case TextureType::AMBIENT_OCCLUSION:
            return "AO";
        case TextureType::EMISSION:
            return "Emission";
        case TextureType::HEIGHT:
            return "Height";
        case TextureType::CUSTOM:
            return "Custom";
        default:
            return "Unknown";
        }
    }

    TextureType StringToTextureType(const char *str)
    {
        if (strcmp(str, "Diffuse") == 0)
            return TextureType::DIFFUSE;
        if (strcmp(str, "Normal") == 0)
            return TextureType::NORMAL;
        if (strcmp(str, "Specular") == 0)
            return TextureType::SPECULAR;
        if (strcmp(str, "Metallic") == 0)
            return TextureType::METALLIC;
        if (strcmp(str, "Roughness") == 0)
            return TextureType::ROUGHNESS;
        if (strcmp(str, "AO") == 0)
            return TextureType::AMBIENT_OCCLUSION;
        if (strcmp(str, "Emission") == 0)
            return TextureType::EMISSION;
        if (strcmp(str, "Height") == 0)
            return TextureType::HEIGHT;
        return TextureType::CUSTOM;
    }

    void Material::Use()
    {
        if (m_shader)
        {
            m_shader->Use();
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
        ApplyTextures(); // 确保在使用材质时应用所有纹理
    }

    void Material::SetShader(const std::string &shaderName)
    {
        throw std::runtime_error("SetShader with string name is not implemented yet.");
    }

    void Material::SetShader(Shader *shader)
    {
        if (shader)
        {
            m_shader = shader;
            m_shader->Use();
        }
        else
        {
            Logger::Error("Attempted to set a null shader.");
        }
    }

    void Material::SetTexture(const std::string &textureName, int slot)
    {
        if (m_shader)
        {
            m_shader->SetInt(textureName.c_str(), slot, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetFloat(const std::string &name, float value)
    {
        if (m_shader)
        {
            m_shader->SetFloat(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetInt(const std::string &name, int value)
    {
        if (m_shader)
        {
            m_shader->SetInt(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetVector(const std::string &name, const glm::vec3 &value)
    {
        if (m_shader)
        {
            m_shader->SetVector3f(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetMatrix(const std::string &name, const glm::mat4 &value)
    {
        if (m_shader)
        {
            m_shader->SetMatrix4(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    const std::string &Material::GetName() const
    {
        return m_name;
    }

    void Material::SetTexture(const std::string &name, Texture *texture, int slot, TextureType type)
    {
        if (!texture)
        {
            Logger::Error("Attempted to set a null texture.");
            return;
        }

        // 检查是否已经存在同名纹理
        auto it = m_textureNameToIndex.find(name);
        if (it != m_textureNameToIndex.end())
        {
            // 如果存在，更新绑定信息
            m_textureBindings[it->second].texture = texture;
            m_textureBindings[it->second].slot = slot;
            m_textureBindings[it->second].type = type;
        }
        else
        {
            // 如果不存在，添加新的绑定
            TextureBinding binding{name, type, (unsigned int)slot, texture};
            m_textureBindings.push_back(binding);
            m_textureNameToIndex[name] = m_textureBindings.size() - 1;
        }
    }

    Texture *Material::GetTexture(const std::string &name) const
    {
        auto it = m_textureNameToIndex.find(name);
        if (it != m_textureNameToIndex.end())
        {
            return m_textureBindings[it->second].texture;
        }
        return nullptr; // 如果没有找到，返回nullptr
    }

    const std::vector<TextureBinding> &Material::GetAllTextures() const
    {
        return m_textureBindings;
    }

    std::vector<TextureBinding> Material::GetTexturesByType(TextureType type) const
    {
        std::vector<TextureBinding> result;
        for (const auto &binding : m_textureBindings)
        {
            if (binding.type == type)
            {
                result.push_back(binding);
            }
        }
        return result;
    }

    bool Material::HasTexture(const std::string &name) const
    {
        return m_textureNameToIndex.find(name) != m_textureNameToIndex.end();
    }

    void Material::RemoveTexture(const std::string &name)
    {
        auto it = m_textureNameToIndex.find(name);
        if (it != m_textureNameToIndex.end())
        {
            unsigned int index = it->second;
            m_textureBindings.erase(m_textureBindings.begin() + index);
            m_textureNameToIndex.erase(it);

            // 更新索引
            for (size_t i = index; i < m_textureBindings.size(); ++i)
            {
                m_textureNameToIndex[m_textureBindings[i].name] = i;
            }
        }
        else
        {
            Logger::Error("Texture with name '{}' not found in material '{}'.", name, m_name);
        }
    }

    size_t Material::GetTextureCount() const
    {
        return m_textureBindings.size();
    }

    TextureBinding *Material::GetTextureAtIndex(size_t index)
    {
        if (index < m_textureBindings.size())
        {
            return &m_textureBindings[index];
        }
        Logger::Error("Index out of bounds when accessing texture at index {} in material '{}'.", index, m_name);
        return nullptr; // 返回nullptr表示索引无效
    }
    void Material::ApplyTextures() const
    {
        if (!m_shader)
        {
            Logger::Error("Material shader is not set.");
            return;
        }

        for (const auto &binding : m_textureBindings)
        {
            if (binding.texture)
            {
                binding.texture->Bind(binding.slot);
                m_shader->SetInt(binding.name.c_str(), binding.slot, true);
                // Logger::Log("Applied texture '{}' of type '{}' to slot {} in material '{}'.",
                //             binding.name, TextureTypeToString(binding.type), binding.slot, m_name);
            }
            else
            {
                Logger::Error("Texture '{}' is null in material '{}'.", binding.name, m_name);
            }
        }
    }
}
