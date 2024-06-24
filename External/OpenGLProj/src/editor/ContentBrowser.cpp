//
// Created by zhengshulin on 2024/6/24.
//

#include "ContentBrowser.h"
#include "imgui.h"
#include "res_path.h"
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

ContentBrowser::ContentBrowser(const std::string &name, EditorScene *scene) : EditorWidget(name, scene) {}

void ContentBrowser::Draw() {
    ImGui::Begin(name.c_str());
    ImGui::Text("Content");
    ListFile();
    ImGui::End();
}

pair<vector<string>,vector<string>> ListFilesAndDirs(const char *path) {
    filesystem::path root(path);
    vector<string> files;
    vector<string> directories;

    for (const auto &entry: filesystem::directory_iterator(root)) {
        if (entry.is_directory()) {
            directories.push_back(entry.path().string());
        } else if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    return {directories,files};
}

void ContentBrowser::ListFile() {
    const char *roots[2] = {"res", "src"};
    auto root =filesystem::path(RES_PATH);
    for (const auto& dir : currentPath) {
        root /= dir;
    }
    ShowDirectory(root.string());


}

void ContentBrowser::ShowDirectory(const string &path) {
    auto [dirs, files] = ListFilesAndDirs(path.c_str());

    // Display directories
    for (const auto &dir : dirs) {
        bool isOpen = ImGui::TreeNodeEx(dir.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
        if (isOpen) {
            if (ImGui::IsItemClicked()) {
                currentPath.push_back(dir);
                ImGui::SetNextItemOpen(true); // Ensure the next item is open
            }
            ImGui::TreePop();
        }
    }

    // Display files
    for (const auto &file : files) {
        if (ImGui::Selectable(file.c_str())) {
            std::cout << "Selected file: " << file << std::endl;
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("CONTENT_BROWSER_FILE", file.c_str(), file.size() + 1);
            ImGui::Text("Dragging %s", file.c_str());
            ImGui::EndDragDropSource();
        }
    }

    if (!currentPath.empty()) {
        ImGui::Separator();
        if (ImGui::Button("Back")) {
            currentPath.pop_back();
        }
    }
}
