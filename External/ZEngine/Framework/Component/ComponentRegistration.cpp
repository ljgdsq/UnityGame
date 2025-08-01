#include "Framework/Component/ComponentRegistry.h"

// 组件头文件
#include "Framework/Component/Transform.h"
#include "Framework/Component/Camera.h"
#include "Framework/Graphic/MeshRenderer.h"
#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Component/Light/Light.h"

namespace framework
{

    void RegisterAllComponents()
    {
        // todo : 可以使用宏来自动注册所有组件
        auto &registry = ComponentRegistry::GetInstance();

        // 注册所有可用的组件
        // 注意：Transform 通常是必须的，可能不需要手动添加
        // registry.RegisterComponent<Transform>("Transform");

        // 渲染相关组件
        registry.RegisterComponent<MeshRenderer>("MeshRenderer");
        registry.RegisterComponent<MeshFilter>("MeshFilter");
        registry.RegisterComponent<Transform>("Transform");

        // 相机组件
        registry.RegisterComponent<Camera>("Camera");

        // 光照组件
        registry.RegisterComponent<Light>("Light");

        // 可以继续添加更多组件...
        // registry.RegisterComponent<YourCustomComponent>("Your Custom Component");
    }
}
