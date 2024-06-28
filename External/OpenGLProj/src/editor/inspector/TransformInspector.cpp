//
// Created by zhengshulin on 2024/6/21.
//

#include "TransformInspector.h"
#include "imgui.h"
#include "core/GameObject.h"
void TransformInspector::Inspect(GameObject *node) {
    if (node) {
        ImGui::Text("Transform:");

        float min_input_width = 50.0f;
        float available_width = ImGui::GetContentRegionAvail().x;
        float input_width = (available_width - ImGui::CalcTextSize("Position X: Y: Z: ").x - 20) / 3.0f;

        input_width = input_width > min_input_width ? input_width : min_input_width;
        ImGui::PushItemWidth(input_width);
        {
            ImGui::Text("Position");
            ImGui::SameLine();

            // Display the "X" label and input
            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##X", &(node->transform.position.x), 1.0f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &(node->transform.position.y), 1.0f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &(node->transform.position.z), 1.0f);
        }

        {

            ImGui::Text("Rotation");
            ImGui::SameLine();

            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##RX", &(node->transform.rotate.x), 1.0f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##RY", &(node->transform.rotate.y), 1.0f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##RZ", &(node->transform.rotate.z), 1.0f);
        }

        {

            ImGui::Text("Scale   ");
            ImGui::SameLine();

            ImGui::Text("X:");
            ImGui::SameLine();
            ImGui::DragFloat("##SX", &(node->transform.scale.x), 0.1f);
            ImGui::SameLine();

            // Display the "Y" label and input
            ImGui::Text("Y:");
            ImGui::SameLine();
            ImGui::DragFloat("##SY", &(node->transform.scale.y), 0.1f);
            ImGui::SameLine();

            // Display the "Z" label and input
            ImGui::Text("Z:");
            ImGui::SameLine();
            ImGui::DragFloat("##SZ", &(node->transform.scale.z), 0.1f);
        }


        ImGui::PopItemWidth();

    }

}
