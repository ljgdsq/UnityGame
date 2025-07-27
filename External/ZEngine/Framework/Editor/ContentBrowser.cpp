#include "Framework/Editor/ContentBrowser.h"
#include "Framework/Editor/AssetDragDropSystem.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Editor/EditorContext.h"
#include "Framework/Editor/ThumbnailManager.h"
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
        EditorContext::GetInstance().isDrag = false;

        ImGui::Separator();
        // Display files
        for (const auto &file : files)
        {

            std::filesystem::path filePath(file);
            std::string filename = filePath.filename().string();
            std::string extension = filePath.extension().string();
            // 使用新的资源拖拽系统
            if (RenderAssetDragSource(file, filename, extension))
            {
                EditorContext::GetInstance().OnFileSelected(file);
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

    bool ContentBrowser::RenderAssetDragSource(const std::string &filePath, const std::string &filename, const std::string &extension)
    {
        // 检查文件扩展名以确定资源类型
        std::shared_ptr<framework::Asset> asset = framework::AssetManager::GetInstance().LoadAsset<Asset>(filePath);

        if (asset)
        {
            void *thumbnailId = asset->GetThumbnailTextureId();
            if (!thumbnailId)
            {
                auto defaultThumbnail = ThumbnailManager::GetInstance().GetThumbnail(asset->GetType());
                if (defaultThumbnail)
                {
                    thumbnailId = defaultThumbnail->GetThumbnailTextureId();
                }
                else
                {
                    LOG_WARN("No default thumbnail found for asset type: {}", AssetTypeToString(asset->GetType()));
                    thumbnailId = nullptr;
                }
            }

            // 使用基础的拖拽系统
            if (auto textureAsset = std::dynamic_pointer_cast<framework::TextureAsset>(asset))
            {

                return AssetDragDropSystem::RenderDragSource(filename, DragDropType::Texture,
                                                             textureAsset->GetAssetId(),
                                                             textureAsset->GetName(),
                                                             thumbnailId);
            }
            else if (auto meshAsset = std::dynamic_pointer_cast<framework::MeshAsset>(asset))
            {
                return AssetDragDropSystem::RenderDragSource(filename, DragDropType::Asset,
                                                             meshAsset->GetAssetId(),
                                                             meshAsset->GetName(),
                                                             thumbnailId);
            }
            else
            {
                return AssetDragDropSystem::RenderDragSource(filename, DragDropType::Unknown,
                                                             asset->GetAssetId(),
                                                             asset->GetName(),
                                                             thumbnailId);
            }
        }
        return false;
    }

}
