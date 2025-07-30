#include "Framework/Graphic/ShaderReflection.h"
#include "glad/glad.h"
namespace framework
{
    ShaderReflection::ShaderReflection(int shaderId)
        : m_shaderId(shaderId)
    {
        Reflect();
    }

    ShaderReflection::~ShaderReflection()
    {
    }

    std::vector<ShaderReflection::Uniform> ShaderReflection::GetUniforms() const
    {
        return m_uniforms;
    }

    std::vector<ShaderReflection::Sampler> ShaderReflection::GetSamplers() const
    {
        return m_samplers;
    }

    void ShaderReflection::Reflect()
    {
        m_uniforms.clear();
        m_samplers.clear();

        GLint uniformCount = 0;
        glGetProgramiv(m_shaderId, GL_ACTIVE_UNIFORMS, &uniformCount);
        GLint maxNameLength = 0;
        glGetProgramiv(m_shaderId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
        std::vector<char> nameData(maxNameLength);

        for (GLint i = 0; i < uniformCount; ++i)
        {
            GLsizei nameLen = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveUniform(m_shaderId, i, maxNameLength, &nameLen, &size, &type, nameData.data());
            std::string name(nameData.data(), nameLen);
            GLint location = glGetUniformLocation(m_shaderId, name.c_str());

            // 判断是否为sampler类型（即texture参数）
            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_2D_ARRAY)
            {
                Sampler sampler;
                sampler.name = name;
                sampler.location = location;
                m_samplers.push_back(sampler);
            }else{
                Uniform uniform;
                uniform.name = name;
                uniform.location = location;
                uniform.type = type;
                uniform.size = size;
                m_uniforms.push_back(uniform);
            }
        }
    }
}
