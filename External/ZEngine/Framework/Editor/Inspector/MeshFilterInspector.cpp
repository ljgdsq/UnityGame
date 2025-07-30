#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Editor/Inspector/MeshFilterInspector.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"
#include "Framework/Core/Texture.h"
#include "Framework/Editor/AssetDragDropSystem.h"
using namespace framework;
namespace editor
{
    void MeshFilterInspector::Inspect(framework::GameObject *node)
    {
        framework::MeshFilter *meshFilter = node->GetComponent<framework::MeshFilter>();
        if (!meshFilter)
        {
            LOG_ERROR("MeshFilterInspector: no mesh filter component found on GameObject: {}", node->GetName());
            return;
        }

        // Render component title
        ImGui::Text("Mesh Filter");
        ImGui::Separator();
        RenderMeshInfo(meshFilter);
    }

    std::string MeshFilterInspector::GetComponentDisplayName() const
    {
        return "Mesh Filter";
    }

    void MeshFilterInspector::RenderMeshInfo(framework::MeshFilter *meshFilter)
    {
        // 配置资源字段
        AssetFieldConfig config;
        config.showPreview = true;
        config.allowNull = true;
        config.previewSize = ImVec2(64, 64);
        config.nullText = "None (Mesh)";
        config.selectText = "Select Mesh";

        auto meshAsset = meshFilter->GetMeshAsset();
        if (!meshAsset)
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "no mesh");
            RenderTextureThumbnail(reinterpret_cast<void *>(static_cast<intptr_t>(Texture::GetDefaultMissingTexture()->GetTextureID())), config.previewSize);
            DragDropPayload payload;
            if (AssetDragDropSystem::AcceptDragDrop(DragDropType::Mesh, payload))
            {
                if (payload.IsValid())
                {
                    auto asset = framework::AssetManager::GetInstance().GetAsset<framework::MeshAsset>(payload.dataId);
                    if (asset)
                    {
                        meshFilter->SetMesh(asset);
                    }
                }
            }
            return;
        }

        // 显示网格信息
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Mesh Information:"); // 显示网格名称
        ImGui::Text("Name: %s", meshAsset->GetName().c_str());

        // 显示网格ID
        ImGui::Text("ID: %s", meshAsset->GetAssetId().c_str());

        if (meshAsset->IsLoaded())
        {
            ImGui::Text("Status: Loaded");

            if (RenderMeshField("Mesh Preview:", meshAsset, config))
            {
                meshFilter->SetMesh(meshAsset);
            }
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Status: Loading...");
        }
    }
}
