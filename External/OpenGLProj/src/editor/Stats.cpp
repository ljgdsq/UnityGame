//
// Created by zhengshulin on 2024/6/21.
//

#include "Stats.h"
#include "imgui.h"



void Stats::Draw() {
    static ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin(name.c_str());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

Stats::Stats(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {


}
