#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Core/Texture.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"
#include "Framework/Editor/Inspector/MaterialAssetInspector.h"
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
        if (materialAsset)
        {
            auto material = materialAsset->GetMaterial();
            ImGui::Text("Material: %s", material->GetName().c_str());
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));       // 透明
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0)); // 透明
                                                                              // 记录区域起始位置
            if (ImGui::CollapsingHeader("Material Asset Details", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap))
            {
                materialAssetInspector->Inspect(materialAsset);


                ImVec2 rectMin = ImGui::GetItemRectMin();

    ImGui::BeginGroup();
    materialAssetInspector->Inspect(materialAsset);
    ImGui::EndGroup();

        ImVec2 rectMax = ImGui::GetItemRectMax();
    float padding = 6.0f;
    // 绘制淡蓝色背景（覆盖整个折叠区域）
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(rectMin.x - padding, rectMin.y - padding),
        ImVec2(rectMax.x + padding, rectMax.y + padding),
        ImColor(180, 200, 255, 60), 6.0f
    );
            }



            ImGui::PopStyleColor(2);
        }
        else
        {
            ImGui::Text("No material assigned.");
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
