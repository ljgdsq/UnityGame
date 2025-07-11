#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Render/Material.h"
using namespace framework;
namespace editor
{
    void MeshRendererInspector::Inspect(GameObject *node)
    {
        if (!node || !node->HasComponent<MeshRenderer>())
            return;

        MeshRenderer *meshRenderer = node->GetComponent<MeshRenderer>();
        ImGui::Text("Mesh Renderer  %s", node->GetName().c_str());

        RenderMaterial(node);
    }

    void MeshRendererInspector::RenderMaterial(GameObject *node)
    {
        MeshRenderer *meshRenderer = node->GetComponent<MeshRenderer>();
        if (!meshRenderer)
            return;

        Material *material = meshRenderer->GetMaterial();
        if (material)
        {
            ImGui::Text("Material: %s", material->GetName().c_str());
            // 这里可以添加更多关于材质的属性编辑功能
        }
        else
        {
            ImGui::Text("No material assigned.");
        }
    }
} // namespace editor