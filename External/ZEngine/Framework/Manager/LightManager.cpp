#include "Framework/Manager/LightManager.h"
#include "Framework/Graphic/Material.h"
namespace framework
{
    std::vector<Light *> LightManager::s_lights;

    void LightManager::RegisterLight(Light *light)
    {
        if (light && std::find(s_lights.begin(), s_lights.end(), light) == s_lights.end())
        {
            s_lights.push_back(light);
        }else{
            Logger::Warn("LightManager: Attempted to register a null or already registered light.");
        }
    }

    void LightManager::UnregisterLight(Light *light)
    {
        auto it = std::remove(s_lights.begin(), s_lights.end(), light);
        if (it != s_lights.end())
        {
            s_lights.erase(it, s_lights.end());
        }
    }
    void LightManager::ApplyLights(std::shared_ptr<Material> material)
    {
        if (!material)
            return;
        ApplyLights(material.get());
    }

    void LightManager::ApplyLights(Material *material)
    {
        if (!material)
            return;

        // 清除之前的光源数据
        material->ClearLights();

        // 遍历所有光源并应用到材质
        for (const auto &light : s_lights)
        {
            if (light)
            {
                material->AddLight(light);
            }
        }
    }

    int LightManager::GetLightCount()
    {
        return static_cast<int>(s_lights.size());
    }

}
