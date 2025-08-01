#include "Framework/Editor/Inspector/TransformInspector.h"
#include "Framework/Component/Transform.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Manager/CameraManager.h"
#include "ImGuizmo.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace framework;
namespace editor
{

    namespace
    {
        static std::unordered_map<Transform *, glm::vec3> eulerCache; // todo remove Transform
    }
    void TransformInspector::Inspect(GameObject *node)
    {

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

            // X: 红色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.2f, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0.3f, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.4f, 0.4f, 1));
            ImGui::Button("X");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##X", &(transform->position.x), 1.0f);
            ImGui::SameLine();

            // Y: 绿色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 1, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 1, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1, 0.4f, 1));
            ImGui::Button("Y");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &(transform->position.y), 1.0f);
            ImGui::SameLine();

            // Z: 蓝色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 1, 1));
            ImGui::Button("Z");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &(transform->position.z), 1.0f);

            auto view = CameraManager::GetInstance().GetMainCamera()->GetViewMatrix();
            auto proj = CameraManager::GetInstance().GetMainCamera()->GetProjectionMatrix();

            glm::mat4 model = transform->GetModelMatrix();
            float matrix[16];
            memcpy(matrix, glm::value_ptr(model), sizeof(matrix));

            if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
                                     ImGuizmo::TRANSLATE, ImGuizmo::WORLD, matrix))
            {
                // 如果用户拖动了gizmo，更新transform
                glm::mat4 newModel = glm::make_mat4(matrix);
                // 提取位置信息，如果Y轴反向就在这里处理
                glm::vec3 newPosition = glm::vec3(newModel[3]);
                transform->position = newPosition;
            }
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

            // X: 红色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.2f, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0.3f, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.4f, 0.4f, 1));
            ImGui::Button("X");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##RX", &eulerAngles.x, 1.0f);
            ImGui::SameLine();

            // Y: 绿色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 1, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 1, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1, 0.4f, 1));
            ImGui::Button("Y");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##RY", &eulerAngles.y, 1.0f);
            ImGui::SameLine();

            // Z: 蓝色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 1, 1));
            ImGui::Button("Z");
            ImGui::PopStyleColor(3);
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

            // X: 红色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.2f, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0.3f, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.4f, 0.4f, 1));
            ImGui::Button("X");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##SX", &(transform->scale.x), 0.1f);
            ImGui::SameLine();

            // Y: 绿色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 1, 0.2f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 1, 0.3f, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1, 0.4f, 1));
            ImGui::Button("Y");
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat("##SY", &(transform->scale.y), 0.1f);
            ImGui::SameLine();

            // Z: 蓝色按钮标签
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 1, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 1, 1));
            ImGui::Button("Z");
            ImGui::PopStyleColor(3);
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
