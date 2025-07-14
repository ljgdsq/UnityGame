#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Component/Light/Light.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>

namespace editor
{
    class LightInspector : public TypedComponentInspector<framework::Light>
    {
    public:
        explicit LightInspector() : TypedComponentInspector<framework::Light>() {}

        // Override the Inspect method to handle Light components
        void Inspect(framework::GameObject *node) override;

        // 组件显示名称
        std::string GetComponentDisplayName() const override;

    private:
        void RenderLightProperties(framework::Light *light);
    };
}
