//
// Created by zhengshulin on 2024/6/24.
//

#include "imgui.h"
#include "EditorMenuBar.h"
#include "EditorScene.h"
#include "ImGuiFileDialog.h"
#include "utils/FileUtil.h"
#include "core/Archiver.h"
#include "log/Logger.h"
#include <sstream>
static bool show_demo_window = false;

void ShowImGuiDemo();

void EditorMenuBar::DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
//                scene->Save();
                ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".cpp,.h,.txt");
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
                Logger::error("About not impl!");
            }
            if (ImGui::MenuItem("ImGui Demo")) {
                show_demo_window = !show_demo_window;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey")) {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

            if (FileUtil::FileExists(filePathName)){
                FileUtil::RemoveFile(filePathName);
            }
            std::ostringstream ss;
          Archiver<EditorScene> archiver;
            auto res=archiver.save(scene);

            printf("EditorScene :%s",res.c_str());
//            FileUtil::WriteText(filePathName,)
            // Your application save function
//            scene->SaveToFile(filePathName);

            // action
        }
        // close
        ImGuiFileDialog::Instance()->Close();
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

