#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Graphic/Shader.h"

namespace framework{
    class ShaderAsset : public Asset{
    public:
        ShaderAsset(const std::string name);
        ~ShaderAsset() override = default;

        long GetSize() const override;

        rapidjson::Value Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const override;

        void Deserialize(const rapidjson::Value &json) override;

        void GenerateThumbnail() override;

        const std::shared_ptr<Shader> &GetShader() const;

        void SetShader(const std::shared_ptr<Shader> &shader);

    private:
        std::shared_ptr<Shader> m_shader;
    };
}