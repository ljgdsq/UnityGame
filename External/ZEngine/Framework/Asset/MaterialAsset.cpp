#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Graphic/Material.h"
namespace framework
{

    MaterialAsset::MaterialAsset(const std::string &name)
        : Asset(name, AssetType::Material), m_name(name)
    {
    }

    MaterialAsset::~MaterialAsset() = default;

    const std::string &MaterialAsset::GetName() const
    {
        return m_name;
    }

    void MaterialAsset::SetShader(std::shared_ptr<Shader> shader)
    {
        m_shader = shader;
    }

    std::shared_ptr<Shader> MaterialAsset::GetShader() const
    {
        return m_shader;
    }

    rapidjson::Value MaterialAsset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);
        jsonValue.AddMember("name", rapidjson::Value(m_name.c_str(), allocator), allocator);
        if (m_shader)
        {
            jsonValue.AddMember("shader", rapidjson::Value(m_shader->shaderPath.c_str(), allocator), allocator);
        }

        if (m_material)
        {
            jsonValue.AddMember("filePath", rapidjson::Value(GetFilePath().c_str(), allocator), allocator);

            std::vector<framework::Material::UnifiedTextureBinding> bindings = m_material->GetAllTextureBindings();
            rapidjson::Value textureBinding(rapidjson::kArrayType);
            for (const auto &binding : bindings)
            {
                    rapidjson::Value bindingValue(rapidjson::kObjectType);
                    bindingValue.AddMember("name", rapidjson::Value(binding.name.c_str(), allocator), allocator);
                    bindingValue.AddMember("slot", binding.slot, allocator);
                    bindingValue.AddMember("type", static_cast<int>(binding.type), allocator);
                    if (binding.textureAsset){
                        bindingValue.AddMember("texture", rapidjson::Value(binding.textureAsset->GetTexture()->GetFilePath().c_str(), allocator), allocator);
                    }
                    textureBinding.PushBack(bindingValue, allocator);
            }
            jsonValue.AddMember("binding", textureBinding, allocator);

        }


        return jsonValue;
    }

    void MaterialAsset::Deserialize(const rapidjson::Value &json)
    {



        if (json.HasMember("name") && json["name"].IsString())
        {
            m_name = json["name"].GetString();
        }
        if (json.HasMember("shader") && json["shader"].IsString())
        {
            std::string shaderPath = json["shader"].GetString();
            auto asset = AssetManager::GetInstance().LoadAsset<ShaderAsset>(shaderPath);
            if (!asset)
            {
                // Handle error: shader not found
                LOG_ERROR("Shader not found: {}", shaderPath);
            }
            else
            {
                m_shader = asset->GetShader();
            }
            m_material = std::make_shared<Material>(m_name);
            m_material->SetShader(m_shader.get());
        }
        else
        {
            m_shader.reset();
        }

        if (json.HasMember("filePath") && json["filePath"].IsString())
        {
            SetFilePath(json["filePath"].GetString());
        }

        if (json.HasMember("binding") && json["binding"].IsArray())
        {
            const auto &bindings = json["binding"];
            for (int i =0;i<bindings.Size();i++)
            {
                const auto& binding=bindings[i];
                if (binding.IsObject() && binding.HasMember("name") && binding.HasMember("slot") && binding.HasMember("type"))
                {
                    std::string name = binding["name"].GetString();
                    int slot = binding["slot"].GetInt();
                    TextureType type = static_cast<TextureType>(binding["type"].GetInt());
                    std::shared_ptr<TextureAsset> textureAsset;
                    if (binding.HasMember("texture") && binding["texture"].IsString())
                    {
                        std::string texturePath = binding["texture"].GetString();
                        textureAsset = AssetManager::GetInstance().LoadAsset<TextureAsset>(texturePath);
                    }
                    m_material->SetTexture(name, textureAsset, slot, type);
                }
            }
        }

    }

    long MaterialAsset::GetSize() const
    {
        return 0;
    }

    void MaterialAsset::GenerateThumbnail()
    {
    }

} // namespace framework
