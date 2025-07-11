#include "Framework/Editor/ContentBrowser.h"
#include <filesystem>
#include <iostream>
#include <utility>
#include "Framework/Log/Logger.h"
using namespace std;
namespace editor
{

    pair<vector<string>, vector<string>> ListFilesAndDirs(const char *path)
    {
        filesystem::path root(path);
        vector<string> files;
        vector<string> directories;

        for (const auto &entry : filesystem::directory_iterator(root))
        {
            if (entry.is_directory())
            {
                directories.push_back(entry.path().string());
            }
            else if (entry.is_regular_file())
            {
                files.push_back(entry.path().string());
            }
        }

        return {directories, files};
    }

    ContentBrowser::ContentBrowser() : EditorWidget("Content Browser")
    {
    }

    ContentBrowser::~ContentBrowser()
    {
    }

    void ContentBrowser::Initialize()
    {
        rootPath = (std::filesystem::current_path()/ "Res").string();
        Logger::Debug("ContentBrowser initialized with path: {}", rootPath);
    }

    void ContentBrowser::Update(float deltaTime)
    {
    }

    void ContentBrowser::Render()
    {
        ImGui::Begin(name.c_str());
        ImGui::Text("Content");
        RenderFileSystem();
        ImGui::End();
    }

    void ContentBrowser::Shutdown()
    {
    }

    void ContentBrowser::RenderFileSystem()
    {
        const char *rootPathCStr ="Res";
        auto root= std::filesystem::path(std::filesystem::current_path());
        for (const auto &c : currentPath)
        {
            root /= c;
        }

        std::filesystem::path path(root);
        auto [dirs, files] = ListFilesAndDirs(path.string().c_str());
        // Display directories
        for (const auto &dir : dirs)
        {
            
            std::filesystem::path dirPath(dir);
            bool isOpen = ImGui::TreeNodeEx(dirPath.filename().string().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
            if (isOpen)
            {
                if (ImGui::IsItemClicked())
                {
                    currentPath.push_back(dir);
                    ImGui::SetNextItemOpen(true); // Ensure the next item is open
                }
                ImGui::TreePop();
            }
        }

        // Display files
        for (const auto &file : files)
        {
            std::filesystem::path filePath(file);
            if (ImGui::Selectable(filePath.filename().string().c_str()))
            {
                //std::cout << "Selected file: " << file << std::endl;
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                // 确保字符串正确终止
                ImGui::SetDragDropPayload("CONTENT_BROWSER_FILE", file.c_str(), file.size() + 1);
                ImGui::Text("Dragging: %s", filePath.filename().string().c_str());
                ImGui::EndDragDropSource();
            }
        }

        if (!currentPath.empty())
        {
            ImGui::Separator();
            if (ImGui::Button("Back"))
            {
                currentPath.pop_back();
            }
        }
    }
    void editor::ContentBrowser::RenderAsset(const std::string &assetPath) {}

}
