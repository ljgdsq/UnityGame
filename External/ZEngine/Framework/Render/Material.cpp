#include "Framework/Render/Material.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Shader.h"
namespace framework
{
    void Material::Use()
    {
        if (m_shader)
        {
            m_shader->Use();
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetShader(const std::string &shaderName)
    {
        throw std::runtime_error("SetShader with string name is not implemented yet.");
    }

    void Material::SetShader(Shader *shader)
    {
        if (shader)
        {
            m_shader = shader;
        }
        else
        {
            Logger::Error("Attempted to set a null shader.");
        }
    }


    void Material::SetTexture(const std::string &textureName, int slot)
    {
       
        
    }

    void Material::SetFloat(const std::string &name, float value)
    {
        if (m_shader)
        {
            m_shader->SetFloat(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetInt(const std::string &name, int value)
    {
        if (m_shader)
        {
            m_shader->SetInt(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetVector(const std::string &name, const glm::vec3 &value)
    {
        if (m_shader)
        {
            m_shader->SetVector3f(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    void Material::SetMatrix(const std::string &name, const glm::mat4 &value)
    {
        if (m_shader)
        {
            m_shader->SetMatrix4(name.c_str(), value, true);
        }
        else
        {
            Logger::Error("Material shader is not set.");
        }
    }

    const std::string &Material::GetName() const
    {
        return m_name;
    }

}