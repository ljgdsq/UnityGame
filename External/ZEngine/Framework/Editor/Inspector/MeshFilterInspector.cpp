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

    void MeshFilterInspector::RenderMeshAssetField(framework::MeshFilter *meshFilter)
    {
    }

    void MeshFilterInspector::RenderMeshInfo(framework::MeshFilter *meshFilter)
    {
        auto meshAsset = meshFilter->GetMeshAsset();
        if (!meshAsset)
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "没有分配网格资源");
            return;
        }

        // 显示网格信息
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "网格信息:"); // 显示网格名称
        ImGui::Text("名称: %s", meshAsset->GetName().c_str());

        // 显示网格ID
        ImGui::Text("ID: %s", meshAsset->GetAssetId().c_str());

        // 显示网格统计信息（如果有的话）
        // 这里假设MeshAsset有获取顶点数和面数的方法
        // 实际实现可能需要根据MeshAsset的具体接口调整
        if (meshAsset->IsLoaded())
        {
            ImGui::Text("状态: 已加载");
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
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "状态: 正在加载...");
        }
    }
}
