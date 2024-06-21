//
// Created by zhengshulin on 2024/6/21.
//

#include "Inspector.h"
#include "imgui.h"
#include "inspector/ComponentInspector.h"
#include "editor/inspector/TransformInspector.h"

Inspector::Inspector(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {
    componentInspectors.push_back(new TransformInspector());
}

void Inspector::Draw() {
    ImGui::Begin(name.c_str());
    if (node){
        ImGui::Text(inspectName.c_str());
    }
    for (const auto &inspect :componentInspectors) {
        inspect->Inspect(node);
    }

    ImGui::End();
}

void Inspector::InspectNode(SceneNode *node) {
    inspectName = node->GetName();
    this->node=node;
}
