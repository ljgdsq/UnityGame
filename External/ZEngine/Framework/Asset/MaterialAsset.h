#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Asset/AssetReference.h"
#include "Framework/Asset/TextureAsset.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace framework
{
    /**
     * @brief 材质资源类
     * 统一管理材质资源，支持序列化、缩略图、依赖关系等
     */
    class MaterialAsset : public Asset
    {
    public:
        MaterialAsset(const std::string &name);
        MaterialAsset(const std::string &name, const std::string &assetId);
        virtual ~MaterialAsset() override;

        // Asset interface
        void Load() override;
        void Unload() override;
        long GetSize() const override;
        void GenerateThumbnail() override;

        // Serialization
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &json) override;

        // Material management
        std::shared_ptr<Material> GetMaterial() const;
        void SetMaterial(std::shared_ptr<Material> material);

        // Shader management
        void SetShader(const std::string &shaderName);
        std::string GetShaderName() const;

        // Texture management
        void SetTexture(const std::string &name, AssetReference<TextureAsset> textureRef, int slot = 0);
        void SetTexture(const std::string &name, const std::string &textureAssetId, int slot = 0);
        AssetReference<TextureAsset> GetTextureReference(const std::string &name) const;

        // Material properties
        void SetFloat(const std::string &name, float value);
        void SetInt(const std::string &name, int value);
        void SetVector(const std::string &name, const glm::vec3 &value);
        void SetMatrix(const std::string &name, const glm::mat4 &value);

        float GetFloat(const std::string &name) const;
        int GetInt(const std::string &name) const;
        glm::vec3 GetVector(const std::string &name) const;
        glm::mat4 GetMatrix(const std::string &name) const;

        // File operations
        bool LoadFromFile(const std::string &filePath);
        bool SaveToFile(const std::string &filePath) const;

        // Create default material
        void CreateDefault();

    private:
        // Material data
        std::shared_ptr<Material> m_material;
        std::string m_shaderName;

        // Texture references
        std::unordered_map<std::string, AssetReference<TextureAsset>> m_textureReferences;

        // Material properties
        std::unordered_map<std::string, float> m_floatProperties;
        std::unordered_map<std::string, int> m_intProperties;
        std::unordered_map<std::string, glm::vec3> m_vectorProperties;
        std::unordered_map<std::string, glm::mat4> m_matrixProperties;

        // Helper methods
        void ApplyPropertiesToMaterial();
        void ApplyTexturesToMaterial();
        void UpdateMaterialFromProperties();

        // Dependency management
        void UpdateDependencies();
        void OnTextureChanged(const std::string &textureName);
    };

} // namespace framework
