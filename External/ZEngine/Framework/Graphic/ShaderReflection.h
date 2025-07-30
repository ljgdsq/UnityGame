#pragma once
#include <vector>
#include <string>
namespace framework
{
    class ShaderReflection
    {
    public:
        ShaderReflection(int shaderId);
        ~ShaderReflection();

        struct Uniform
        {
            std::string name;
            int type;
            int location;
            int size;
        };

        struct Sampler
        {
            std::string name;
            int location;
        };

        std::vector<Uniform> GetUniforms() const;
        std::vector<Sampler> GetSamplers() const;

    private:
        int m_shaderId;
        std::vector<Uniform> m_uniforms;
        std::vector<Sampler> m_samplers;

        void Reflect();
    };
} // namespace framework
