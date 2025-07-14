#include "Framework/Editor/Inspector/LightInspector.h"
using namespace framework;

namespace editor
{

    void LightInspector::Inspect(GameObject *node)
    {
        if (!node || !node->HasComponent<Light>())
            return;

        Light *light = node->GetComponent<Light>();
        ImGui::Text("Light Inspector for %s", node->GetName().c_str());

        // Render light properties
        RenderLightProperties(light);
    }

    void LightInspector::RenderLightProperties(Light *light)
    {
        if (!light)
            return;

        // Light Type
        int currentType = static_cast<int>(light->GetLightType());
        const char *lightTypes[] = {"Directional", "Point", "Spot"};
        if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes)))
        {
            light->SetLightType(static_cast<LightType>(currentType));
        }

        // Light Color
        glm::vec3 color = light->GetColor();
        if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
        {
            light->SetColor(color);
        }

        // Light Intensity
        float intensity = light->GetIntensity();
        if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 100.0f))
        {
            light->SetIntensity(intensity);
        }
    }

    // 新增方法实现
    std::string LightInspector::GetComponentDisplayName() const
    {
        return "Light";
    }

    bool LightInspector::IsComponentEnabled(framework::GameObject *obj) const
    {
        if (!obj || !obj->HasComponent<Light>())
            return false;

        Light *light = obj->GetComponent<Light>();
        return light && light->IsEnabled();
    }

    void LightInspector::SetComponentEnabled(framework::GameObject *obj, bool enabled)
    {
        if (!obj || !obj->HasComponent<Light>())
            return;

        Light *light = obj->GetComponent<Light>();
        if (light)
            light->SetEnabled(enabled);
    }

    bool LightInspector::RemoveComponent(framework::GameObject *obj)
    {
        if (!obj || !obj->HasComponent<Light>())
            return false;
        obj->RemoveComponent<Light>();
        return true;
    }

} // namespace editor
