#include "Framework/Editor/Inspector/TransformInspector.h"
#include "Framework/Component/Transform.h"
#include "Framework/Core/GameObject.h"
using namespace framework;
namespace editor
{

    namespace
    {
        static std::unordered_map<Transform *, glm::vec3> eulerCache; // todo remove Transform
    }
    void TransformInspector::Inspect(GameObject *node)
    {
        Transform *transform = node->GetComponent<Transform>();

        ImGui::Text("Transform  %s", node->GetName().c_str());

        float min_input_width = 50.0f;
        float available_width = ImGui::GetContentRegionAvail().x;
        float input_width = (available_width - ImGui::CalcTextSize("Position X: Y: Z: ").x - 20) / 3.0f;

        input_width = input_width > min_input_width ? input_width : min_input_width;
        ImGui::PushItemWidth(input_width);
        RenderPosition(node);
        RenderRotation(node);
        RenderScale(node);

        ImGui::PopItemWidth();
    }

    void TransformInspector::RenderPosition(GameObject *node)
    {
        Transform *transform = node->GetComponent<Transform>();
        glm::vec3 position = transform->GetPosition();

        {
            ImGui::Text("Position");
            ImGui::SameLine();

            // Display the "X" label and input
            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##X", &(transform->position.x), 1.0f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &(transform->position.y), 1.0f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &(transform->position.z), 1.0f);
        }
    }
    void TransformInspector::RenderRotation(GameObject *node)
    {
        Transform *transform = node->GetComponent<Transform>();

        // 只在首次访问或外部修改时更新缓存
        if (eulerCache.find(transform) == eulerCache.end())
        {
            eulerCache[transform] = glm::degrees(glm::eulerAngles(transform->rotation));
        }

        glm::vec3 &eulerAngles = eulerCache[transform];
        glm::vec3 originalEuler = eulerAngles;

        {

            ImGui::Text("Rotation");
            ImGui::SameLine();

            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##RX", &eulerAngles.x, 1.0f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##RY", &eulerAngles.y, 1.0f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##RZ", &eulerAngles.z, 1.0f);
        }

        // 只有用户修改时才更新四元数
        if (eulerAngles != originalEuler)
        {
            transform->SetRotation(eulerAngles);
            // 保持缓存同步
            eulerCache[transform] = eulerAngles;
        }
    }

    void TransformInspector::RenderScale(GameObject *node)
    {
        Transform *transform = node->GetComponent<Transform>();

        {

            ImGui::Text("Scale   ");
            ImGui::SameLine();

            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##SX", &(transform->scale.x), 0.1f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##SY", &(transform->scale.y), 0.1f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##SZ", &(transform->scale.z), 0.1f);
        }
    }

    // 新增方法实现
    std::string TransformInspector::GetComponentDisplayName() const
    {
        return "Transform";
    }

}
