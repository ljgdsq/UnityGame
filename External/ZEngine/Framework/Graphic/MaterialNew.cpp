#include "Framework/Graphic/MaterialNew.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Graphic/Shader.h"
#include <algorithm>

// todo: remove CameraManager
#include "Framework/Component/Light/Light.h"
#include "Framework/Manager/CameraManager.h"
#include <exception>
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
        throw new std::exception("not impl");
    }

    void Material::SetTexture(const std::string &name, std::shared_ptr<TextureAsset> textureAsset, int slot, TextureType type)
    {
        AssetTextureBinding *binding= FindTextureBinding(name);
        if (binding){
            binding ->asset = textureAsset;
            binding->slot = slot;
            binding->type = type;
            Logger::Debug("Material: Updated texture asset binding: {}", name);
        }else{
            AssetTextureBinding newBinding;
            newBinding.name = name;
            newBinding.asset = textureAsset;
            newBinding.slot = slot;
            newBinding.type = type;

            m_textureBindings.push_back(newBinding);
            UpdateTextureNameToIndexMapping();
            Logger::Debug("Material: Added new texture asset binding: {}", name);
        }
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
        throw new std::exception("not impl");
    }



    std::shared_ptr<TextureAsset> Material::GetTextureAsset(const std::string &name) const
    {
        const AssetTextureBinding *binding = FindTextureBinding(name);
        return binding ? binding->asset : nullptr;
    }

    std::shared_ptr<Texture> Material::GetTextureSharedPtr(const std::string &name) const
    {
        // 查找AssetReference中的纹理
        const AssetTextureBinding *binding = FindTextureBinding(name);
        if (binding)
        {
            auto textureAsset = binding->asset;
            if (textureAsset && textureAsset->IsLoaded())
            {
                return textureAsset->GetTexture();
            }
        }

        return nullptr;
    }

    Texture *Material::GetTexture(const std::string &name) const
    {
        // 查找AssetReference中的纹理
        const AssetTextureBinding *binding = FindTextureBinding(name);
        if (binding)
        {
            auto textureAsset = binding->asset;
            if (textureAsset && textureAsset->IsLoaded())
            {
                auto texture = textureAsset->GetTexture();
                return texture.get(); // 返回原始指针
            }
        }

        return nullptr;
    }

    std::vector<Material::UnifiedTextureBinding> Material::GetAllTextureBindings() const
    {
        std::vector<UnifiedTextureBinding> result;

        // 添加AssetReference纹理绑定
        for (const auto &binding : m_textureBindings)
        {
            result.emplace_back(binding);
        }

        return result;
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
        return FindTextureBinding(name) != nullptr;
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
    }

    size_t Material::GetTextureCount() const
    {
        return m_textureBindings.size();
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
        if (!m_shader)
        {
            Logger::Error("Material shader is not set when applying lights.");
            return;
        }

        // Logger::Log("Applying lights to shader '{}' in material '{}'.", m_shader->id, m_name);
        // Logger::Log("Number of lights to apply: {}", m_lights.size());

        // 设置光源数量
        int lightCount = std::min(static_cast<int>(m_lights.size()), 8); // 限制最多8个光源
        m_shader->SetInt("u_lightCount", lightCount, true);

        // 应用每个光源的数据
        for (int i = 0; i < lightCount; ++i)
        {
            const Light *light = m_lights[i];
            if (!light)
                continue;

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
                float outerAngle = glm::cos(glm::radians(15.0f)); // 30度锥角
                m_shader->SetFloat((lightPrefix + ".spotInnerAngle").c_str(), innerAngle, true);
                m_shader->SetFloat((lightPrefix + ".spotOuterAngle").c_str(), outerAngle, true);
            }
            else
            {
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

        auto camera = CameraManager::GetInstance().GetMainCamera();
        auto trans = camera->GetGameObject()->GetTransform();

        m_shader->SetVector3f("u_viewPosition", trans->GetPosition(), true);

        // 设置默认材质属性
        glm::vec3 diffuseColor(1.0f, 1.0f, 1.0f);  // 白色漫反射
        glm::vec3 specularColor(0.5f, 0.5f, 0.5f); // 灰色镜面反射
        float shininess = 32.0f;                   // 默认光泽度

        m_shader->SetVector3f("u_diffuseColor", diffuseColor, true);
        m_shader->SetVector3f("u_specularColor", specularColor, true);
        m_shader->SetFloat("u_shininess", shininess, true);
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

                rapidjson::Value textureAssetValue = binding.asset->Serialize(allocator);
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
                        binding.asset->Deserialize(bindingJson["textureAsset"]);
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

        for (const auto &binding : m_textureBindings)
        {
            auto textureAsset = binding.asset;
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

} // namespace framework
