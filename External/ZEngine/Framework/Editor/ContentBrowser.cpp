#include "Framework/Editor/ContentBrowser.h"
#include "Framework/Editor/AssetDragDropSystem.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
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
        rootPath = (std::filesystem::current_path() / "Res").string();
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

        auto root = std::filesystem::path(std::filesystem::current_path());
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
            std::string filename = filePath.filename().string();
            std::string extension = filePath.extension().string();

            if (ImGui::Selectable(filename.c_str()))
            {
                // std::cout << "Selected file: " << file << std::endl;
            }

            // 使用新的资源拖拽系统
            RenderAssetDragSource(file, filename, extension);
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

    void ContentBrowser::RenderAssetDragSource(const std::string &filePath, const std::string &filename, const std::string &extension)
    {
        // 检查文件扩展名以确定资源类型
        std::shared_ptr<framework::Asset> asset = nullptr;

        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == ".tga")
        {
            // 尝试加载纹理资源
            asset = framework::AssetManager::GetInstance().LoadAsset<framework::TextureAsset>(filePath);
        }
        else if (extension == ".obj" || extension == ".fbx" || extension == ".gltf" || extension == ".glb")
        {
            // 尝试加载网格资源
            asset = framework::AssetManager::GetInstance().LoadAsset<framework::MeshAsset>(filePath);
        }

        if (asset)
        {
            // 使用基础的拖拽系统
            if (auto textureAsset = std::dynamic_pointer_cast<framework::TextureAsset>(asset))
            {
                void *thumbnailId = textureAsset->GetThumbnailTextureId();
                AssetDragDropSystem::RenderDragSource(filename, DragDropType::Texture,
                                                      textureAsset->GetAssetId(),
                                                      textureAsset->GetName(),
                                                      thumbnailId);
            }
            else if (auto meshAsset = std::dynamic_pointer_cast<framework::MeshAsset>(asset))
            {
                void *thumbnailId = meshAsset->GetThumbnailTextureId();
                AssetDragDropSystem::RenderDragSource(filename, DragDropType::Asset,
                                                      meshAsset->GetAssetId(),
                                                      meshAsset->GetName(),
                                                      thumbnailId);
            }
        }
        else
        {
            // 通用文件拖拽（用于不支持的文件类型）
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("CONTENT_BROWSER_FILE", filePath.c_str(), filePath.size() + 1);
                ImGui::Text("拖拽: %s", filename.c_str());
                ImGui::EndDragDropSource();
            }
        }
    }

}
