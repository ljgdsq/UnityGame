//
// Created by zhengshulin on 2024/6/21.
//

#include "Hierarchy.h"
#include "imgui.h"
#include "editor/EditorScene.h"

void Hierarchy::Draw() {
    nodeNames.clear();
    auto childs= scene->GetNodes();
   for(const auto &child:childs){
       nodeNames.push_back(child->GetName());
   }

   static ImGuiIO& io = ImGui::GetIO();
   ImGui::Begin(name.c_str());
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int n = 0; n < childs.size(); n++)
        {
            const bool is_selected = (selectIndex == n);
            if (ImGui::Selectable(nodeNames[n].c_str(), is_selected)){
                selectIndex = n;
                scene->InspectNode(childs[n]);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

   ImGui::End();
}

Hierarchy::Hierarchy(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {}
