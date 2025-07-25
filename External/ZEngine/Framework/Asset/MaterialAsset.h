#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Graphic/Shader.h"
namespace framework
{
    class Material;
    class MaterialAsset : public Asset
    {
    public:
        MaterialAsset(const std::string &name);
        virtual ~MaterialAsset() override;

        // 获取材质名称
        const std::string &GetName() const;

        // 设置和获取着色器
        void SetShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> GetShader() const;

        // 序列化和反序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &json) override;

        // 获取资源大小
        long GetSize() const override;
        // 生成缩略图
        void GenerateThumbnail() override;
        std::shared_ptr<Material> GetMaterial() const { return m_material; }

    private:
        std::string m_name;                   // 材质名称
        std::shared_ptr<Shader> m_shader;     // 关联的着色器
        std::shared_ptr<Material> m_material; // 材质实例
    };

} // namespace framework
