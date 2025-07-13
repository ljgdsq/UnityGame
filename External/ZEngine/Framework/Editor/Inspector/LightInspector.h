#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Component/Light/Light.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
namespace editor{
    class LightInspector : public ComponentInspector
    {
    public:
        explicit LightInspector() : ComponentInspector() {}

        // Override the Inspect method to handle Light components
        void Inspect(framework::GameObject *node) override
        {
            if (!node || !node->HasComponent<framework::Light>())
                return;

            framework::Light *light = node->GetComponent<framework::Light>();
            ImGui::Text("Light Inspector for %s", node->GetName().c_str());

            // Render light properties
            RenderLightProperties(light);
        }

    private:
        void RenderLightProperties(framework::Light *light)
        {
            if (!light)
                return;

            // Light Type
            int currentType = static_cast<int>(light->GetLightType());
            const char* lightTypes[] = { "Directional", "Point", "Spot" };
            if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
                light->SetLightType(static_cast<framework::LightType>(currentType));
            }

            // Light Color
            glm::vec3 color = light->GetColor();
            if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
                light->SetColor(color);
            }

            // Light Intensity
            float intensity = light->GetIntensity();
            if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 100.0f)) {
                light->SetIntensity(intensity);
            }
        }
    };
}