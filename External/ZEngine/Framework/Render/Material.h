#pragma once
#include <string>
#include "glm/glm.hpp"
namespace framework {
    class Material {
    public:
        Material() = default;
        virtual ~Material() = default;
        void Use();
        // 设置着色器
        virtual void SetShader(const std::string& shaderName);
        virtual void SetShader(class Shader* shader);
        // 设置纹理
        virtual void SetTexture(const std::string& textureName, int slot = 0);

        // 设置材质属性
        virtual void SetFloat(const std::string& name, float value) ;
        virtual void SetInt(const std::string& name, int value);
        virtual void SetVector(const std::string& name, const glm::vec3& value);

        virtual void SetMatrix(const std::string& name, const glm::mat4& value);

        // 获取材质名称
        virtual const std::string& GetName() const ;


        private:
        class Shader* m_shader = nullptr; // 使用指针避免循环依赖
        std::string m_name; // 材质名称
    };

}