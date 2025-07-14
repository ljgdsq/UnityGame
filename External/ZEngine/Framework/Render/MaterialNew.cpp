#include "Framework/Render/MaterialNew.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Shader.h"
#include <algorithm>

namespace framework
{
    void Material::Use()
    {
        if (m_shader)
        {
            m_shader->Use();
            ApplyTextures();
            ApplyLights();
        }
    }

    void Material::SetShader(const std::string &shaderName)
    {
        // 这里应该通过ShaderManager或AssetManager获取shader
        // 暂时保留原有逻辑
        Logger::Debug("Material: Setting shader: {}", shaderName);
    }

    void Material::SetShader(class Shader *shader)
    {
        m_shader = shader;
        Logger::Debug("Material: Set shader directly");
    }

    void Material::SetTexture(const std::string &name, const std::string &textureAssetId, int slot, TextureType type)
    {
        AssetReference<TextureAsset> textureAssetRef(textureAssetId);
        SetTexture(name, textureAssetRef, slot, type);
    }

    void Material::SetTexture(const std::string &name, std::shared_ptr<TextureAsset> textureAsset, int slot, TextureType type)
    {
        if (textureAsset)
        {
            AssetReference<TextureAsset> textureAssetRef(textureAsset->GetAssetId());
            SetTexture(name, textureAssetRef, slot, type);
        }
        else
        {
            RemoveTexture(name);
        }
    }

    void Material::SetTexture(const std::string &name, const AssetReference<TextureAsset> &textureAssetRef, int slot, TextureType type)
    {
        // 查找现有的纹理绑定
        AssetTextureBinding *binding = FindTextureBinding(name);

        if (binding)
        {
            // 更新现有绑定
            binding->textureAsset = textureAssetRef;
            binding->slot = slot;
            binding->type = type;
        }
        else
        {
            // 创建新的绑定
            AssetTextureBinding newBinding;
            newBinding.name = name;
            newBinding.textureAsset = textureAssetRef;
            newBinding.slot = slot;
            newBinding.type = type;

            m_textureBindings.push_back(newBinding);
            UpdateTextureNameToIndexMapping();
        }

        Logger::Debug("Material: Set texture asset '{}' for slot '{}' (slot {}, type {})",
                      textureAssetRef.GetAssetId(), name, slot, static_cast<int>(type));
    }

    void Material::SetTexture(const std::string &textureName, int slot)
    {
        // 兼容性方法 - 通过名称查找纹理
        Logger::Debug("Material: SetTexture compatibility method called with name: {}", textureName);

        // 这里可以尝试通过AssetManager查找纹理
        // 暂时保留原有行为
    }

    void Material::SetTexture(const std::string &name, Texture *texture, int slot, TextureType type)
    {
        // 兼容性方法 - 直接设置纹理对象
        DirectTextureBinding *binding = FindDirectTextureBinding(name);

        if (binding)
        {
            binding->texture = texture;
            binding->slot = slot;
            binding->type = type;
        }
        else
        {
            DirectTextureBinding newBinding;
            newBinding.name = name;
            newBinding.texture = texture;
            newBinding.slot = slot;
            newBinding.type = type;

            m_directTextureBindings.push_back(newBinding);
        }

        Logger::Debug("Material: Set texture directly for slot '{}' (slot {}, type {})",
                      name, slot, static_cast<int>(type));
    }

    AssetReference<TextureAsset> Material::GetTextureAssetReference(const std::string &name) const
    {
        const AssetTextureBinding *binding = FindTextureBinding(name);
        return binding ? binding->textureAsset : AssetReference<TextureAsset>();
    }

    std::shared_ptr<TextureAsset> Material::GetTextureAsset(const std::string &name) const
    {
        const AssetTextureBinding *binding = FindTextureBinding(name);
        return binding ? binding->textureAsset.Get() : nullptr;
    }

    std::shared_ptr<Texture> Material::GetTextureSharedPtr(const std::string &name) const
    {
        // 优先查找AssetReference中的纹理
        const AssetTextureBinding *binding = FindTextureBinding(name);
        if (binding)
        {
            auto textureAsset = binding->textureAsset.Get();
            if (textureAsset && textureAsset->IsLoaded())
            {
                return textureAsset->GetTexture();
            }
        }

        // 查找直接设置的纹理（兼容性）
        const DirectTextureBinding *directBinding = FindDirectTextureBinding(name);
        if (directBinding && directBinding->texture)
        {
            return std::shared_ptr<Texture>(directBinding->texture, [](Texture *)
                                            {
                                                // 不删除，因为这是外部管理的指针
                                            });
        }

        return nullptr;
    }

    Texture *Material::GetTexture(const std::string &name) const
    {
        // 优先查找AssetReference中的纹理
        const AssetTextureBinding *binding = FindTextureBinding(name);
        if (binding)
        {
            auto textureAsset = binding->textureAsset.Get();
            if (textureAsset && textureAsset->IsLoaded())
            {
                auto texture = textureAsset->GetTexture();
                return texture.get(); // 返回原始指针
            }
        }

        // 查找直接设置的纹理（兼容性）
        const DirectTextureBinding *directBinding = FindDirectTextureBinding(name);
        if (directBinding && directBinding->texture)
        {
            return directBinding->texture;
        }

        return nullptr;
    }

    const std::vector<AssetTextureBinding> &Material::GetAllTextureBindings() const
    {
        return m_textureBindings;
    }

    std::vector<AssetTextureBinding> Material::GetTexturesByType(TextureType type) const
    {
        std::vector<AssetTextureBinding> result;

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
        return FindTextureBinding(name) != nullptr || FindDirectTextureBinding(name) != nullptr;
    }

    void Material::RemoveTexture(const std::string &name)
    {
        // 从AssetReference绑定中移除
        auto it = std::find_if(m_textureBindings.begin(), m_textureBindings.end(),
                               [&name](const AssetTextureBinding &binding)
                               {
                                   return binding.name == name;
                               });

        if (it != m_textureBindings.end())
        {
            m_textureBindings.erase(it);
            UpdateTextureNameToIndexMapping();
            Logger::Debug("Material: Removed texture asset binding: {}", name);
        }

        // 从直接绑定中移除
        auto directIt = std::find_if(m_directTextureBindings.begin(), m_directTextureBindings.end(),
                                     [&name](const DirectTextureBinding &binding)
                                     {
                                         return binding.name == name;
                                     });

        if (directIt != m_directTextureBindings.end())
        {
            m_directTextureBindings.erase(directIt);
            Logger::Debug("Material: Removed direct texture binding: {}", name);
        }
    }

    size_t Material::GetTextureCount() const
    {
        return m_textureBindings.size() + m_directTextureBindings.size();
    }

    AssetTextureBinding *Material::GetTextureAtIndex(size_t index)
    {
        if (index < m_textureBindings.size())
        {
            return &m_textureBindings[index];
        }
        return nullptr;
    }

    const AssetTextureBinding *Material::GetTextureAtIndex(size_t index) const
    {
        if (index < m_textureBindings.size())
        {
            return &m_textureBindings[index];
        }
        return nullptr;
    }

    void Material::SetFloat(const std::string &name, float value)
    {
        if (m_shader)
        {
            m_shader->SetFloat(name.c_str(), value);
        }
    }

    void Material::SetInt(const std::string &name, int value)
    {
        if (m_shader)
        {
            m_shader->SetInt(name.c_str(), value);
        }
    }

    void Material::SetVector(const std::string &name, const glm::vec3 &value)
    {
        if (m_shader)
        {
            m_shader->SetVector3f(name.c_str(), value);
        }
    }

    void Material::SetMatrix(const std::string &name, const glm::mat4 &value)
    {
        if (m_shader)
        {
            m_shader->SetMatrix4(name.c_str(), value);
        }
    }

    const std::string &Material::GetName() const
    {
        return m_name;
    }

    void Material::ClearLights()
    {
        m_lights.clear();
    }

    void Material::AddLight(class Light *light)
    {
        if (light)
        {
            m_lights.push_back(light);
        }
    }

    void Material::ApplyLights() const
    {
        // 应用光源到着色器
        // 这里需要根据具体的光源系统实现
        Logger::Debug("Material: Applied {} lights", m_lights.size());
    }

    rapidjson::Value Material::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);

        // 序列化基本信息
        rapidjson::Value nameValue(m_name.c_str(), allocator);
        jsonValue.AddMember("name", nameValue, allocator);

        // 序列化纹理绑定
        if (!m_textureBindings.empty())
        {
            rapidjson::Value textureBindingsValue(rapidjson::kArrayType);

            for (const auto &binding : m_textureBindings)
            {
                rapidjson::Value bindingValue(rapidjson::kObjectType);

                rapidjson::Value bindingNameValue(binding.name.c_str(), allocator);
                bindingValue.AddMember("name", bindingNameValue, allocator);

                rapidjson::Value textureAssetValue = binding.textureAsset.Serialize(allocator);
                bindingValue.AddMember("textureAsset", textureAssetValue, allocator);

                bindingValue.AddMember("slot", binding.slot, allocator);
                bindingValue.AddMember("type", static_cast<int>(binding.type), allocator);

                textureBindingsValue.PushBack(bindingValue, allocator);
            }

            jsonValue.AddMember("textureBindings", textureBindingsValue, allocator);
        }

        return jsonValue;
    }

    void Material::Deserialize(const rapidjson::Value &json)
    {
        // 反序列化基本信息
        if (json.HasMember("name") && json["name"].IsString())
        {
            m_name = json["name"].GetString();
        }

        // 反序列化纹理绑定
        if (json.HasMember("textureBindings") && json["textureBindings"].IsArray())
        {
            const auto &textureBindingsArray = json["textureBindings"];

            for (rapidjson::SizeType i = 0; i < textureBindingsArray.Size(); i++)
            {
                const auto &bindingJson = textureBindingsArray[i];
                if (bindingJson.IsObject())
                {
                    AssetTextureBinding binding;

                    if (bindingJson.HasMember("name") && bindingJson["name"].IsString())
                    {
                        binding.name = bindingJson["name"].GetString();
                    }

                    if (bindingJson.HasMember("textureAsset") && bindingJson["textureAsset"].IsObject())
                    {
                        binding.textureAsset.Deserialize(bindingJson["textureAsset"]);
                    }

                    if (bindingJson.HasMember("slot") && bindingJson["slot"].IsInt())
                    {
                        binding.slot = bindingJson["slot"].GetInt();
                    }

                    if (bindingJson.HasMember("type") && bindingJson["type"].IsInt())
                    {
                        binding.type = static_cast<TextureType>(bindingJson["type"].GetInt());
                    }

                    m_textureBindings.push_back(binding);
                }
            }

            UpdateTextureNameToIndexMapping();
        }
    }

    void Material::ApplyTextures() const
    {
        if (!m_shader)
        {
            return;
        }

        // 应用AssetReference纹理
        for (const auto &binding : m_textureBindings)
        {
            auto textureAsset = binding.textureAsset.Get();
            if (textureAsset && textureAsset->IsLoaded())
            {
                auto texture = textureAsset->GetTexture();
                if (texture)
                {
                    texture->Bind(binding.slot);
                    m_shader->SetInt(binding.name.c_str(), binding.slot);
                }
            }
        }

        // 应用直接设置的纹理（兼容性）
        for (const auto &binding : m_directTextureBindings)
        {
            if (binding.texture)
            {
                binding.texture->Bind(binding.slot);
                m_shader->SetInt(binding.name.c_str(), binding.slot);
            }
        }
    }

    void Material::UpdateTextureNameToIndexMapping()
    {
        m_textureNameToIndex.clear();

        for (size_t i = 0; i < m_textureBindings.size(); ++i)
        {
            m_textureNameToIndex[m_textureBindings[i].name] = i;
        }
    }

    AssetTextureBinding *Material::FindTextureBinding(const std::string &name)
    {
        auto it = std::find_if(m_textureBindings.begin(), m_textureBindings.end(),
                               [&name](const AssetTextureBinding &binding)
                               {
                                   return binding.name == name;
                               });

        return it != m_textureBindings.end() ? &(*it) : nullptr;
    }

    const AssetTextureBinding *Material::FindTextureBinding(const std::string &name) const
    {
        auto it = std::find_if(m_textureBindings.begin(), m_textureBindings.end(),
                               [&name](const AssetTextureBinding &binding)
                               {
                                   return binding.name == name;
                               });

        return it != m_textureBindings.end() ? &(*it) : nullptr;
    }

    Material::DirectTextureBinding *Material::FindDirectTextureBinding(const std::string &name)
    {
        auto it = std::find_if(m_directTextureBindings.begin(), m_directTextureBindings.end(),
                               [&name](const DirectTextureBinding &binding)
                               {
                                   return binding.name == name;
                               });

        return it != m_directTextureBindings.end() ? &(*it) : nullptr;
    }

    const Material::DirectTextureBinding *Material::FindDirectTextureBinding(const std::string &name) const
    {
        auto it = std::find_if(m_directTextureBindings.begin(), m_directTextureBindings.end(),
                               [&name](const DirectTextureBinding &binding)
                               {
                                   return binding.name == name;
                               });

        return it != m_directTextureBindings.end() ? &(*it) : nullptr;
    }

} // namespace framework
