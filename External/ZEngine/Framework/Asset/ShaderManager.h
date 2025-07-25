#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "Framework/Common/Define.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Asset/ShaderAsset.h"

namespace framework
{
    class ShaderManager
    {
        SINGLETON_CLASS(ShaderManager);

    public:
        void Initialize();
        std::shared_ptr<ShaderAsset> GetShader(const std::string &name);
        std::vector<std::shared_ptr<ShaderAsset>> GetAllShaders();
    private:
        std::unordered_map<std::string, std::shared_ptr<ShaderAsset>> m_shaderCache;
    };
}
