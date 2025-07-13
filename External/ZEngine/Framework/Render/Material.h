#pragma once
#include <string>
#include <unordered_map>
#include "Framework/Core/Texture.h"
#include "glm/glm.hpp"
namespace framework
{
    class Material
    {
    public:
        Material(const std::string &name) : m_name(name) {}
        Material() : m_name("DefaultMaterial") {} // 默认构造函数，设置默认名称
        virtual ~Material() = default;
        void Use();
        // 设置着色器
        virtual void SetShader(const std::string &shaderName);
        virtual void SetShader(class Shader *shader);
        // 设置纹理
        virtual void SetTexture(const std::string &textureName, int slot = 0);

        // 设置材质属性
        virtual void SetFloat(const std::string &name, float value);
        virtual void SetInt(const std::string &name, int value);
        virtual void SetVector(const std::string &name, const glm::vec3 &value);

        virtual void SetMatrix(const std::string &name, const glm::mat4 &value);

        // 获取材质名称
        virtual const std::string &GetName() const;

    private:
        class Shader *m_shader = nullptr; // 使用指针避免循环依赖
        std::string m_name;               // 材质名称

        std::vector<TextureBinding> m_textureBindings; // 纹理绑定列表

        std::unordered_map<std::string, unsigned int> m_textureNameToIndex; // 纹理名称到索引的映射

    public:
        // 增强的纹理管理
        void SetTexture(const std::string &name, Texture *texture, int slot = 0, TextureType type = TextureType::DIFFUSE);
        Texture *GetTexture(const std::string &name) const;
        const std::vector<TextureBinding> &GetAllTextures() const;

        // 纹理类型查询
        std::vector<TextureBinding> GetTexturesByType(TextureType type) const;
        bool HasTexture(const std::string &name) const;
        void RemoveTexture(const std::string &name);

        // 检查器特定的辅助方法
        size_t GetTextureCount() const;
        TextureBinding *GetTextureAtIndex(size_t index);

        // Light support methods
        void ClearLights();
        void AddLight(class Light* light);
        void ApplyLights() const;

        private:
          void ApplyTextures() const; // 应用所有纹理到着色器
          
          // Light-related members
          std::vector<class Light*> m_lights; // 当前材质使用的光源列表
    };

}