#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Core/Texture.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"
#include "Framework/Editor/Inspector/MaterialAssetInspector.h"
#include "Framework/Editor/AssetDragDropSystem.h"
using namespace framework;

namespace editor
{

    void MeshRendererInspector::Inspect(GameObject *node)
    {
        if (!node || !node->HasComponent<MeshRenderer>())
            return;

        if (!materialAssetInspector)
        {
            materialAssetInspector = new MaterialAssetInspector();
        }

        MeshRenderer *meshRenderer = node->GetComponent<MeshRenderer>();
        RenderMaterial(node, meshRenderer);
    }

    void MeshRendererInspector::RenderMaterial(GameObject *node, MeshRenderer *meshRenderer)
    {
        float padding = 6.0f;
        auto materialAsset = meshRenderer->GetMaterial();
        {
            ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, 40)); // 空白区域
            ImVec2 rectMin = ImGui::GetItemRectMin();
            ImVec2 rectMax = ImGui::GetItemRectMax();
            ImDrawList *drawList = ImGui::GetWindowDrawList();
            drawList->AddRect(rectMin, rectMax, IM_COL32(180, 180, 180, 128), 6.0f, 0, 2.0f);
            ImGui::SetCursorScreenPos(ImVec2(rectMin.x + 12, rectMin.y + 12));
            ImGui::TextDisabled("Drag Material Here");
            DragDropPayload payload;

            if (AssetDragDropSystem::AcceptDragDrop(DragDropType::Material, payload))
            {
                if (payload.IsValid())
                {
                    auto asset = framework::AssetManager::GetInstance().GetAsset<framework::MaterialAsset>(payload.dataId);
                    if (asset)
                    {
                        meshRenderer->SetMaterial(asset);
                    }
                }
            }
        }
        ImGui::Spacing();

        if (materialAsset)
        {
            auto material = materialAsset->GetMaterial();

            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));       // 透明
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0)); // 透明
                                                                              // 记录区域起始位置
            if (ImGui::CollapsingHeader("Material Asset Details", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap))
            {
                ImGui::BeginGroup();
                ImGui::Indent(20.0f);
                ImGui::Spacing();
                materialAssetInspector->Inspect(materialAsset);
                ImGui::Spacing();
                ImGui::Unindent(20.0f);
                ImGui::EndGroup();
                ImVec2 rectMin = ImGui::GetItemRectMin();
                ImVec2 rectMax = ImGui::GetItemRectMax();
                rectMax.x = rectMin.x + ImGui::GetContentRegionAvail().x-20;
                // 绘制整个区域的边框
                ImDrawList *drawList = ImGui::GetWindowDrawList();
                drawList->AddRect(rectMin, rectMax, IM_COL32(100, 150, 255, 255), 4.0f, 0, 2.0f);
            }

            ImGui::PopStyleColor(2);
        }

    }

    void MeshRendererInspector::RenderMaterialTextures(framework::Material *material)
    {
    }

    // 更新方法
    void MeshRendererInspector::Update(float deltaTime)
    {
        if (materialAssetInspector)
        {
            materialAssetInspector->Update(deltaTime);
        }
    }

    // 新增方法实现
    std::string MeshRendererInspector::GetComponentDisplayName() const
    {
        return "Mesh Renderer";
    }

} // namespace editor
