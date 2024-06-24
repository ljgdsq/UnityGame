//
// Created by zhengshulin on 2024/6/21.
//

#include "EditorPanel.h"
#include "imgui.h"
#include "core/Application.h"
void EditorPanel::Draw() {

    ImGui::Begin(name.c_str());
    bool isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
    Application::setIgnoreInput(!isFocused);
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    if (buffer){
        int textureWidth = buffer->getWidth();
        int textureHeight = buffer->getHeight();
        // 计算保持宽高比缩放后的纹理大小
        float aspectRatio = static_cast<float>(textureWidth) / textureHeight;
        float displayWidth, displayHeight;
        if (windowSize.x / windowSize.y > aspectRatio) {
            // 窗口宽高比大于纹理宽高比，根据高度调整宽度
            displayHeight = windowSize.y;
            displayWidth = displayHeight * aspectRatio;
        } else {
            // 窗口宽高比小于或等于纹理宽高比，根据宽度调整高度
            displayWidth = windowSize.x;
            displayHeight = displayWidth / aspectRatio;
        }

        // 计算纹理显示位置使其居中
        ImVec2 texturePosition = ImGui::GetCursorScreenPos();
        texturePosition.x += (windowSize.x - displayWidth) * 0.5f;
        texturePosition.y += (windowSize.y - displayHeight) * 0.5f;
        ImGui::SetCursorScreenPos(texturePosition);
        ImGui::Image(ImTextureID (buffer->getColorBuffer()),ImVec2(displayWidth, displayHeight));
    }

    ImGui::End();
}

EditorPanel::EditorPanel(const std::string name, EditorScene *scene) : EditorWidget(name, scene) {}

void EditorPanel::ShowGame(void *data) {
    buffer=static_cast<FrameBuffer*>(data);
}
