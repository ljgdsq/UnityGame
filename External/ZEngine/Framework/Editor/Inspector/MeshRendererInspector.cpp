#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Render/Material.h"
#include "Framework/Core/Texture.h"
#include "Framework/Editor/AssetField.h"
#include "Framework/Asset/TextureAsset.h"
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

        ImGui::Text("Material Textures:");
        ImGui::Separator();

        for (const auto &texture : textures)
        {
            ImGui::PushID(texture.slot);

            // 使用新的 AssetField 系统
            auto textureAsset = texture.GetTextureAsset();
            std::shared_ptr<TextureAsset> mutableTextureAsset =
                std::const_pointer_cast<TextureAsset>(textureAsset);

            AssetFieldConfig config;
            config.previewSize = ImVec2(128, 128);
            config.showPreview = true;
            config.allowNull = true;

            // 渲染纹理资源字段
            if (AssetField<TextureAsset>::Render(texture.name, mutableTextureAsset, config))
            {
                // 纹理资源发生变化，更新材质
                if (mutableTextureAsset)
                {
                    material->SetTexture(texture.name, mutableTextureAsset, texture.slot, texture.type);
                    Logger::Debug("Texture asset updated: {} -> {}", texture.name, mutableTextureAsset->GetName());
                }
                else
                {
                    // 清空纹理
                    material->RemoveTexture(texture.name);
                    Logger::Debug("Texture removed: {}", texture.name);
                }
            }

            // 显示纹理类型信息
            const char *textureTypeNames[] = {"Diffuse", "Specular", "Normal", "Height", "Ambient"};
            int typeIndex = static_cast<int>(texture.type);
            if (typeIndex >= 0 && typeIndex < IM_ARRAYSIZE(textureTypeNames))
            {
                ImGui::Text("Type: %s", textureTypeNames[typeIndex]);
            }

            // 显示纹理槽位
            ImGui::Text("Slot: %d", texture.slot);

            ImGui::Separator();
            ImGui::PopID();
        }

        // 添加新纹理绑定按钮
        if (ImGui::Button("Add Texture Binding"))
        {
            // 简化实现：添加一个新的 diffuse 纹理绑定
            std::string newTextureName = "texture_" + std::to_string(textures.size());
            material->SetTexture(newTextureName, nullptr, textures.size(), TextureType::DIFFUSE);
        }
    }

    // 新增方法实现
    std::string MeshRendererInspector::GetComponentDisplayName() const
    {
        return "Mesh Renderer";
    }

} // namespace editor
