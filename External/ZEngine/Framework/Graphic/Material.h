#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Framework/Core/Texture.h"
#include "Framework/Asset/TextureAsset.h"
#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "Framework/Core/SerializableValue.h"

namespace framework
{
    // 纹理绑定结构
    struct AssetTextureBinding
    {
        std::string name;                        // 纹理名称
        int slot = 0;                            // 纹理槽位
        TextureType type = TextureType::DIFFUSE; // 纹理类型
        std::shared_ptr<TextureAsset> asset;
        // 便利方法
        std::shared_ptr<TextureAsset> GetTextureAsset() const { return asset; }
        std::shared_ptr<Texture> GetTexture() const
        {
            return asset ? asset->GetTexture() : nullptr;
        }

        bool IsValid() const { return asset && asset->IsLoaded(); }
    };

    class Material
    {
    public:
        // 统一的纹理绑定结构 - 用于Inspector显示
        struct UnifiedTextureBinding
        {
            std::string name;
            int slot = 0;
            TextureType type = TextureType::DIFFUSE;

            // 纹理获取方法
            std::shared_ptr<Texture> texture;
            std::shared_ptr<TextureAsset> textureAsset;

            UnifiedTextureBinding() = default;

            // 从AssetTextureBinding构造
            UnifiedTextureBinding(const AssetTextureBinding &binding)
                : name(binding.name), slot(binding.slot), type(binding.type), texture(binding.GetTexture()), textureAsset(binding.GetTextureAsset()) {}
        };

    public:
        Material(const std::string &name) : m_name(name) {}
        Material() : m_name("DefaultMaterial") {} // 默认构造函数，设置默认名称
        virtual ~Material() = default;

        void Use();

        // 设置着色器
        void SetShader(std::shared_ptr<class Shader> shader);
        std::shared_ptr<class Shader> GetShader() const{return m_shader;};

        // 纹理管理 - 新的AssetReference版本
        void SetTexture(const std::string &name, const std::string &textureAssetId, int slot = 0, TextureType type = TextureType::DIFFUSE);
        void SetTexture(const std::string &name, std::shared_ptr<TextureAsset> textureAsset, int slot = 0, TextureType type = TextureType::DIFFUSE);

        // 兼容性方法 - 直接设置纹理对象
        virtual void SetTexture(const std::string &textureName, int slot = 0);
        void SetTexture(const std::string &name, Texture *texture, int slot = 0, TextureType type = TextureType::DIFFUSE);

        // 获取纹理
        std::shared_ptr<TextureAsset> GetTextureAsset(const std::string &name) const;
        std::shared_ptr<Texture> GetTextureSharedPtr(const std::string &name) const; // 新方法，返回shared_ptr
        Texture *GetTexture(const std::string &name) const;                          // 兼容性方法，返回原始指针
                                                                                     // 纹理绑定管理

        std::vector<AssetTextureBinding> GetTexturesByType(TextureType type) const;
        bool HasTexture(const std::string &name) const;
        void RemoveTexture(const std::string &name);

        // 统一的纹理绑定接口 - 包含所有类型的纹理
        std::vector<UnifiedTextureBinding> GetAllTextureBindings() const;

        // 检查器特定的辅助方法
        size_t GetTextureCount() const;
        AssetTextureBinding *GetTextureAtIndex(size_t index);
        const AssetTextureBinding *GetTextureAtIndex(size_t index) const;

        // 设置材质属性
        [[deprecated("do not use this")]]
        virtual void SetFloat(const std::string &name, float value);
        [[deprecated("do not use this")]]
        virtual void SetInt(const std::string &name, int value);
        [[deprecated("do not use this")]]
        virtual void SetVector(const std::string &name, const glm::vec3 &value);
        [[deprecated("do not use this")]]
        virtual void SetMatrix(const std::string &name, const glm::mat4 &value);

        template <typename T>
        void SetProperty(const std::string &name, const T &value){
            m_properties[name] = SerializableValue(name,value);
            m_propertiesDirty = true; // 标记属性已修改
        }

        template <typename T>
        T GetProperty(const std::string &name, const T &defaultValue = T{}) const
        {
            auto it = m_properties.find(name);
            return (it != m_properties.end()) ? it->second.GetValue<T>() : defaultValue;
        }

        // 获取材质名称
        virtual const std::string &GetName() const;

        // Light support methods
        void ClearLights();
        void AddLight(class Light *light);
        void ApplyLights() const;


        // 序列化支持
        [[deprecated("do not use this move this to material asset")]]
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;
        void Deserialize(const rapidjson::Value &json);

    private:
        std::shared_ptr<class Shader> m_shader = nullptr; // 使用指针避免循环依赖
        std::string m_name;               // 材质名称

        std::vector<AssetTextureBinding> m_textureBindings;           // 纹理绑定列表（使用AssetReference）
        std::unordered_map<std::string, size_t> m_textureNameToIndex; // 纹理名称到索引的映射

        // Light-related members
        std::vector<class Light *> m_lights; // 当前材质使用的光源列表
        void SetMissingTexturesToDefault(const AssetTextureBinding &binding) const;
        // 辅助方法
        void ApplyTextures() const; // 应用所有纹理到着色器
        void UpdateTextureNameToIndexMapping();
        AssetTextureBinding *FindTextureBinding(const std::string &name);
        const AssetTextureBinding *FindTextureBinding(const std::string &name) const;


        // property management
        std::unordered_map<std::string, SerializableValue> m_properties; // 材质属性
        bool m_propertiesDirty = false; // 属性是否被修改
    };

} // namespace framework
