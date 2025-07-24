#include "Framework/Editor/EditorMenuBar.h"
#include "ImGuiFileDialog.h"
void editor::EditorMenuBar::Initialize() {

}

void editor::EditorMenuBar::Update(float deltaTime) {

}

void editor::EditorMenuBar::Render() {
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
                Logger::Error("About not impl!");
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

//            if (FileUtil::FileExists(filePathName)){
//                FileUtil::RemoveFile(filePathName);
//            }
//
//
//            SceneSerializer sceneSerializer;
//            sceneSerializer.Save(scene);


//            FileUtil::WriteText(filePathName,)
            // Your application save function
//            scene->SaveToFile(filePathName);

            // action
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }


}

void editor::EditorMenuBar::Shutdown() {

}
