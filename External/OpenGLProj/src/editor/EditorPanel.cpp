//
// Created by zhengshulin on 2024/6/21.
//

#include "EditorPanel.h"
#include "imgui.h"

void EditorPanel::Draw() {

    ImGui::Begin(name.c_str());
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    if (buffer){
        ImGui::Image(ImTextureID (buffer->getColorBuffer()),windowSize);
    }

    ImGui::End();
}

EditorPanel::EditorPanel(const std::string name, EditorScene *scene) : EditorWidget(name, scene) {}

void EditorPanel::ShowGame(void *data) {
    buffer=static_cast<FrameBuffer*>(data);
}
