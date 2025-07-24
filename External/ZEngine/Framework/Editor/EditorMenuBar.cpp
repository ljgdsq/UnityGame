#include "Framework/Editor/EditorMenuBar.h"
#include "ImGuiFileDialog.h"
#include "Framework/Common/BaseApplication.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Asset/SceneAsset.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetManager.h"
void editor::EditorMenuBar::Initialize()
{
}

void editor::EditorMenuBar::Update(float deltaTime)
{
}

void editor::EditorMenuBar::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("OpenFileDlgKey", "Choose scene", ".scene");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {

                ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".scene");
            }
            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                framework::BaseApplication::GetInstance()->SetShouldClose(true);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
            }
            if (ImGui::MenuItem("Cut", "Ctrl+X"))
            {
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C"))
            {
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Run"))
        {
            if (ImGui::MenuItem("Start", "F5"))
            {
            }
            if (ImGui::MenuItem("Stop", "Shift+F5"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                Logger::Error("About not impl!");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            auto scene = framework::SceneManager::GetInstance().GetActiveScene();

            if (!scene)
            {
                Logger::Error("No active scene to save.");
                return;
            }

            auto relativePath = std::filesystem::relative(filePathName, std::filesystem::current_path()).string();

            scene->SetPath(relativePath);
            rapidjson::Document::AllocatorType &allocator = rapidjson::Document::AllocatorType();
            auto result = scene->Serialize(allocator);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            result.Accept(writer);

            framework::EngineFileIO::SaveText(filePathName, buffer.GetString());
            LOG_DEBUG("Scene saved to: {}", filePathName.c_str());
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("OpenFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            auto sceneAsset = framework::AssetManager::GetInstance().LoadAsset<framework::SceneAsset>(filePathName);
            if (sceneAsset)
            {
                framework::SceneManager::GetInstance().SetActiveScene(sceneAsset->GetName());
                LOG_DEBUG("Scene loaded: {}", sceneAsset->GetName());
            }
            else
            {
                LOG_ERROR("Failed to load scene from: {}", filePathName);
            }
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void editor::EditorMenuBar::Shutdown()
{
}
