//
// Created by zhengshulin on 2024/6/24.
//

#include <iostream>
#include "TextureInspect.h"
#include "imgui.h"
#include "ui/SceneNode.h"
#include <filesystem>
void TextureInspect::Inspect( SceneNode *node) {
    if (node && node->texture2D.id!=-1 ){
        ImGui::Text("Texture:");
        ImGui::Image((void*)node->texture2D.id,ImVec2(128,128));
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE")) {
            std::string filePath = (const char*)payload->Data;

            node->texture2D=ResourceManager::LoadTexture(filePath.c_str(),std::filesystem::path(filePath).filename().string());

            std::cout << "Loaded new texture from: " << filePath << std::endl;
        }
        ImGui::EndDragDropTarget();
    }

}
