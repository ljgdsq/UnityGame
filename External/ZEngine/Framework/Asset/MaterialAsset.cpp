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
                m_material = std::make_shared<Material>(m_name);
                m_material->SetShader(m_shader.get());
            }
        }
        else
        {
            m_shader.reset();
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
