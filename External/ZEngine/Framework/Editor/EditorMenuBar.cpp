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
#include "Framework/Editor/AssetCreator.h"
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

        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Material"))
            {
                editor::AssetCreator::CreateMaterialAsset("NewMaterial.mat");
            }
            if (ImGui::MenuItem("Empty Scene"))
            {
                editor::AssetCreator::CreateEmptyScene("NewScene.scene");
            }
            if (ImGui::MenuItem("Template Scene"))
            {
                editor::AssetCreator::CreateTemplateScene("NewTemplateScene.scene");
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

            auto asset = new framework::SceneAsset(scene->GetName());
            asset->SetFilePath(relativePath);
            asset->SetScene(std::make_shared<framework::Scene>(*scene));

            rapidjson::Document::AllocatorType allocator;
            auto result = asset->Serialize(allocator);

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
                LOG_DEBUG("Open loaded Scene: {}", sceneAsset->GetName());

                framework::SceneManager::GetInstance().AddScene(sceneAsset->GetName(), sceneAsset->GetScene().get());
                framework::SceneManager::GetInstance().SetActiveScene(sceneAsset->GetName());
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
