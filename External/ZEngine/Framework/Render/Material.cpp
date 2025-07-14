#include "Framework/Render/Material.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Shader.h"
#include "Framework/Component/Light/Light.h"
#include "Framework/Component/Transform.h"
#include "Framework/Manager/CameraManager.h"
namespace framework
{
    // 纹理类型辅助函数实现
    const char *TextureTypeToString(TextureType type)
    {
        switch (type) {
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
        ApplyLights();   // 应用光照数据
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

    void Material::ClearLights()
    {
        m_lights.clear();
    }

    void Material::AddLight(Light* light)
    {
        if (light)
        {
            m_lights.push_back(light);
        }
    }

    void Material::ApplyLights() const
    {
        if (!m_shader)
        {
            Logger::Error("Material shader is not set when applying lights.");
            return;
        }

        // 设置光源数量
        int lightCount = std::min(static_cast<int>(m_lights.size()), 8); // 限制最多8个光源
        m_shader->SetInt("u_lightCount", lightCount, true);

        // 应用每个光源的数据
        for (int i = 0; i < lightCount; ++i)
        {
            const Light* light = m_lights[i];
            if (!light) continue;

            std::string lightPrefix = "u_lights[" + std::to_string(i) + "]";

            // 设置光源类型
            m_shader->SetInt((lightPrefix + ".type").c_str(), static_cast<int>(light->GetLightType()), true);

            // 设置光源颜色和强度
            m_shader->SetVector3f((lightPrefix + ".color").c_str(), light->GetColor(), true);
            m_shader->SetFloat((lightPrefix + ".intensity").c_str(), light->GetIntensity(), true);

            // 获取光源的Transform组件来获取位置和方向
            if (light->GetGameObject())
            {
                auto transform = light->GetGameObject()->GetComponent<Transform>();
                if (transform)
                {
                    // 设置光源位置（对于点光源和聚光灯）
                    glm::vec3 position = transform->GetPosition();
                    m_shader->SetVector3f((lightPrefix + ".position").c_str(), position, true);

                    // 设置光源方向（对于方向光和聚光灯）
                    glm::vec3 forward = transform->GetForward();
                    m_shader->SetVector3f((lightPrefix + ".direction").c_str(), forward, true);
                }
            }

            // 设置衰减参数（可以后续从Light类获取）
            float range = 10.0f;
            if (light->GetLightType() == LightType::Point || light->GetLightType() == LightType::Spot)
            {
                range = 20.0f; // 点光源和聚光灯的默认范围
            }

            m_shader->SetFloat((lightPrefix + ".range").c_str(), range, true);
            m_shader->SetFloat((lightPrefix + ".constant").c_str(), 1.0f, true);
            m_shader->SetFloat((lightPrefix + ".linear").c_str(), 0.09f, true);
            m_shader->SetFloat((lightPrefix + ".quadratic").c_str(), 0.032f, true);

            // 聚光灯参数（如果是聚光灯）
            if (light->GetLightType() == LightType::Spot)
            {
                // 内角和外角的余弦值
                float innerAngle = glm::cos(glm::radians(12.5f)); // 25度锥角
                float outerAngle = glm::cos(glm::radians(15.0f));  // 30度锥角
                m_shader->SetFloat((lightPrefix + ".spotInnerAngle").c_str(), innerAngle, true);
                m_shader->SetFloat((lightPrefix + ".spotOuterAngle").c_str(), outerAngle, true);
            } else {
                // 非聚光灯的默认值
                m_shader->SetFloat((lightPrefix + ".spotInnerAngle").c_str(), -1.0f, true);
                m_shader->SetFloat((lightPrefix + ".spotOuterAngle").c_str(), -1.0f, true);
            }
        }

        // 设置环境光
        glm::vec3 ambientLight(0.2f, 0.2f, 0.2f); // 默认暗淡的环境光
        m_shader->SetVector3f("u_ambientLight", ambientLight, true);

        // 设置相机位置（用于镜面反射计算）
        // TODO: 从CameraManager获取当前相机位置
        glm::vec3 viewPosition(0.0f, 0.0f, 5.0f); // 临时默认位置

        auto camera=CameraManager::GetInstance().GetMainCamera();
        auto trans=camera->GetGameObject()->GetTransform();


        m_shader->SetVector3f("u_viewPosition", trans->GetPosition(), true);

        // 设置默认材质属性
        glm::vec3 diffuseColor(1.0f, 1.0f, 1.0f);  // 白色漫反射
        glm::vec3 specularColor(0.5f, 0.5f, 0.5f); // 灰色镜面反射
        float shininess = 32.0f;                     // 默认光泽度

        m_shader->SetVector3f("u_diffuseColor", diffuseColor, true);
        m_shader->SetVector3f("u_specularColor", specularColor, true);
        m_shader->SetFloat("u_shininess", shininess, true);
    }
}
