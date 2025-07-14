#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Render/Material.h"
#include "Framework/Core/Texture.h"
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
            RenderMaterialTextures(material);
        }
        else
        {
            ImGui::Text("No material assigned.");
        }
    }

    void MeshRendererInspector::RenderMaterialTextures(framework::Material *material)
    {
        auto textures = material->GetAllTextures();
        for (const auto &texture : textures)
        {
            if (texture.texture && texture.texture->GetId() > 0)
            {
                ImGui::Text("Texture:");
                ImGui::Text("width :%d", texture.texture->GetWidth());
                ImGui::SameLine();
                ImGui::Text("height:%d", texture.texture->GetHeight());

                // 创建一个明确的拖拽目标区域
                ImGui::PushID(texture.slot); // 为每个纹理创建唯一ID

                // 绘制纹理图像
                ImGui::Image((void *)texture.texture->GetId(), ImVec2(128, 128));

                // 在图像上设置拖拽目标
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE"))
                    {
                        // 确保payload数据正确终止
                        std::string filePath;
                        if (payload->DataSize > 0)
                        {
                            filePath = (const char *)payload->Data;
                        }

                        Logger::Debug("Drag texture received: {}", filePath.c_str());

                        Texture *newTexture = Texture::LoadTexture(filePath);
                        if (newTexture)
                        {
                            material->SetTexture(texture.name, newTexture, texture.slot);
                            Logger::Debug("Texture set: {}", texture.name.c_str());
                        }
                        else
                        {
                            Logger::Error("Failed to load texture from file: {}", filePath.c_str());
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::Text("Shader Param Name: %s", texture.name.c_str());
                ImGui::Text("Slot :%d", texture.slot);
                ImGui::Separator();

                ImGui::PopID();
            }
        }
    }

    // 新增方法实现
    std::string MeshRendererInspector::GetComponentDisplayName() const
    {
        return "Mesh Renderer";
    }

} // namespace editor
