#pragma once
#include <string>
#include <memory>
#include "Framework/Asset/Asset.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Core/EngineFileIO.h"

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

        // 获取材质实例
        std::shared_ptr<Material> GetMaterial() const { return m_material; }
        // 设置材质实例
        void SetMaterial(std::shared_ptr<Material> material) { m_material = material; }

        // 序列化和反序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &json) override;

        // 获取资源大小
        long GetSize() const override;
        // 生成缩略图
        void GenerateThumbnail() override;

    private:
        std::string m_name;                   // 材质名称
        std::shared_ptr<Shader> m_shader;     // 关联的着色器
        std::shared_ptr<Material> m_material; // 材质实例
    };

} // namespace framework
