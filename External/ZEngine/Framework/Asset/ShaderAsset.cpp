#include "Framework/Asset/ShaderAsset.h"

namespace framework{

    long ShaderAsset::GetSize() const {
        return 0;
    }

    rapidjson::Value ShaderAsset::Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const {
        return Asset::Serialize(allocator);
    }

    void ShaderAsset::Deserialize(const rapidjson::Value &json) {
        Asset::Deserialize(json);
    }

    void ShaderAsset::GenerateThumbnail() {

    }

    const std::shared_ptr<Shader> &ShaderAsset::GetShader() const {
        return m_shader;
    }

    void ShaderAsset::SetShader(const std::shared_ptr<Shader> &shader) {
        m_shader = shader;
    }

    ShaderAsset::ShaderAsset(const std::string name):Asset(name,AssetType::Shader) {

    }
}
