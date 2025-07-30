#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Editor/Inspector/MeshFilterInspector.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"
namespace editor
{
    void MeshFilterInspector::Inspect(framework::GameObject *node)
    {
        framework::MeshFilter *meshFilter = node->GetComponent<framework::MeshFilter>();
        if (!meshFilter)
        {
            LOG_ERROR("MeshFilterInspector: 尝试检查没有MeshFilter组件的GameObject");
            return;
        }

        // 渲染组件标题
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
        auto meshAsset = meshFilter->GetMeshAsset();
        if (!meshAsset)
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "no mesh");
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
            // 配置资源字段
            AssetFieldConfig config;
            config.showPreview = true;
            config.allowNull = true;
            config.previewSize = ImVec2(64, 64);
            config.nullText = "None (Mesh)";
            config.selectText = "Select Mesh";

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
