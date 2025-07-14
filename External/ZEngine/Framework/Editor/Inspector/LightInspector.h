#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Component/Light/Light.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>

namespace editor
{
    class LightInspector : public ComponentInspector
    {
    public:
        explicit LightInspector() : ComponentInspector() {}

        // Override the Inspect method to handle Light components
        void Inspect(framework::GameObject *node) override;

        // 新增：组件信息相关方法
        std::string GetComponentDisplayName() const override;
        bool IsComponentEnabled(framework::GameObject *obj) const override;
        void SetComponentEnabled(framework::GameObject *obj, bool enabled) override;
        bool RemoveComponent(framework::GameObject *obj) override;

    private:
        void RenderLightProperties(framework::Light *light);
    };
}
