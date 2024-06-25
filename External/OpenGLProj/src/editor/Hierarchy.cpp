//
// Created by zhengshulin on 2024/6/21.
//

#include "Hierarchy.h"
#include "imgui.h"
#include "editor/EditorScene.h"
#include "ContextMenu.h"
static bool showpop= false;
static int index=-1;



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
            if (ImGui::Selectable((nodeNames[n] + "##" + std::to_string(n)).c_str(), is_selected, ImGuiButtonFlags_MouseButtonRight|ImGuiButtonFlags_MouseButtonLeft)) {
                selectIndex = n;
                scene->InspectNode(childs[n]);
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)){
                showpop= true;
                index=n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (showpop)
         ImGui::OpenPopup("HierarchyContextMenu");
    if (contextMenu){
        contextMenu->Draw();
    }
    if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left)||ImGui::IsMouseClicked(ImGuiMouseButton_Right) ) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        showpop = false;
    }
   ImGui::End();
}

Hierarchy::Hierarchy(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {
    contextMenu=new ContextMenu("HierarchyContextMenu");
    contextMenu->AddAction("Delete",[=](void*){
        this->scene->getRunningScene()->DeleteChild(index);
        showpop = false;
    },0);

    contextMenu->AddAction("Add",[=](void*){
        if (ImGui::MenuItem("zz")){

        }
        if (ImGui::MenuItem("cc")){

        }
    },0);

}

Hierarchy::~Hierarchy() {
    if (contextMenu)
        delete contextMenu;
}
