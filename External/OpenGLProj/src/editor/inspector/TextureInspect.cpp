//
// Created by zhengshulin on 2024/6/24.
//

#include <iostream>
#include "TextureInspect.h"
#include "imgui.h"
#include <filesystem>
#include "core/GameObject.h"
void TextureInspect::Inspect(GameObject *node) {
//    if (node && node->texture2D && node->texture2D->id!=-1 ){
//        ImGui::Text("Texture:");
//        ImGui::Text("width :%d",node->texture2D->width);ImGui::SameLine();
//        ImGui::Text("height:%d",node->texture2D->height);
//        ImGui::Image((void*)node->texture2D->id,ImVec2(128,128));
//
//    }
//
//    if (ImGui::BeginDragDropTarget()) {
//        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE")) {
//            std::string filePath = (const char*)payload->Data;
//            node->texture2D=ResourceManager::LoadTexture(filePath);
//            std::cout << "Loaded new texture from: " << filePath << std::endl;
//        }
//        ImGui::EndDragDropTarget();
//    }

}
