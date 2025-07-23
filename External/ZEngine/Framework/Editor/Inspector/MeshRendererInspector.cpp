#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Core/Texture.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"

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
        if (!material)
        {
            ImGui::Text("No material available");
            return;
        }

        auto textures = material->GetAllTextureBindings();

        ImGui::Text("Material Textures:");
        ImGui::Separator();

        for (size_t i = 0; i < textures.size(); ++i)
        {
            ImGui::PushID(static_cast<int>(i));

            try
            {
                const auto &texture = textures[i];

                // 直接从材质获取纹理绑定的引用，而不是副本
                AssetTextureBinding *binding = material->GetTextureAtIndex(i);
                if (!binding)
                {
                    ImGui::Text("Texture: %s (No Binding)", texture.name.c_str());
                    ImGui::Text("Slot: %d", texture.slot);
                    ImGui::Separator();
                    ImGui::PopID();
                    continue;
                }

                // 获取当前的纹理资源
                auto currentTextureAsset = binding->asset;
                std::shared_ptr<TextureAsset> mutableTextureAsset = currentTextureAsset;

                // 确保缩略图已生成
                if (currentTextureAsset && currentTextureAsset->IsLoaded() && !currentTextureAsset->HasThumbnail())
                {
                    currentTextureAsset->GenerateThumbnail();
                }

                AssetFieldConfig config;
                config.previewSize = ImVec2(128, 128);
                config.showPreview = true;
                config.allowNull = true;

                // 渲染纹理字段
                if (RenderTextureField("Texture " + std::to_string(i) + ":", mutableTextureAsset, config))
                {
                    // 如果纹理被修改，更新材质
                    if (mutableTextureAsset != currentTextureAsset)
                    {
                        material->SetTexture(binding->name, mutableTextureAsset, binding->slot, binding->type);
                        Logger::Log("MeshRendererInspector: Texture updated for slot %d", binding->slot);
                    }
                }

                // 显示纹理类型信息
                const char *textureTypeNames[] = {"Diffuse", "Specular", "Normal", "Height", "Ambient"};
                int typeIndex = static_cast<int>(binding->type);
                if (typeIndex >= 0 && typeIndex < IM_ARRAYSIZE(textureTypeNames))
                {
                    ImGui::Text("Type: %s", textureTypeNames[typeIndex]);
                }

                // 显示纹理槽位
                ImGui::Text("Slot: %d", binding->slot);
            }
            catch (const std::exception &e)
            {
                Logger::Error("Error in texture rendering: {}", e.what());
                ImGui::Text("Error rendering texture at index %zu", i);
            }

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
