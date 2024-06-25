//
// Created by zhengshulin on 2024/6/24.
//

#include "EditorMenuBar.h"
#include "imgui.h"

static bool show_demo_window = false;

void ShowImGuiDemo();

void DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {

            }
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            }
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Run")) {
            if (ImGui::MenuItem("Start", "F5")) {
            }
            if (ImGui::MenuItem("Stop", "Shift+F5")) {
            }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
            }
            if (ImGui::MenuItem("ImGui Demo")) {
                show_demo_window = !show_demo_window;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ShowImGuiDemo();
}

void EditorMenuBar::Draw() {
    DrawMainMenuBar();
}

EditorMenuBar::EditorMenuBar(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {}

void ShowImGuiDemo() {
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
}

