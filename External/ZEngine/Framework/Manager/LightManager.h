#pragma once
#include "Framework/Component/Light/Light.h"
#include <vector>
namespace framework
{
    // LightManager - 管理场景中的光源
    class LightManager
    {
        private:
        LightManager() = default; // 私有构造函数，禁止实例化

    public:
        static void RegisterLight(Light *light);

        static void UnregisterLight(Light *light);

        // 应用光源到材质
        static void ApplyLights(class Material *material);

        // 获取当前光源数量
        static int GetLightCount();

    private:
        static std::vector<Light*> s_lights; // 存储所有光源的列表
    };
} // namespace framework