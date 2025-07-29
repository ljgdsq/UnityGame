#include "Framework/Editor/Inspector/MaterialAssetInspector.h"
#include "Framework/Asset/ShaderManager.h"
#include "Framework/Editor/AssetEditorDefine.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Asset/MaterialAsset.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Editor/Inspector/AssetField/AssetFieldUI.h"

using namespace framework;
namespace editor
{
    REGISTER_ASSET_INSPECTOR(framework::AssetType::Material, MaterialAssetInspector);
    void MaterialAssetInspector::Update(float deltaTime)
    {
        AssetInspector::Update(deltaTime);
    }

    void MaterialAssetInspector::Inspect(const std::shared_ptr<framework::Asset> &asset)
    {
        AssetInspector::Inspect(asset);
        auto shaders = framework::ShaderManager::GetInstance().GetAllShaders();
        auto materialAsset = std::dynamic_pointer_cast<framework::MaterialAsset>(asset);
        if (!materialAsset)
        {
            ImGui::Text("Invalid Material Asset");
            return;
        }

        ImGui::Spacing();

        static int selectedShaderIndex = 0;
        ImGui::SetNextItemWidth(180);

        if (ImGui::BeginCombo("##ShaderCombo", shaders[selectedShaderIndex]->GetName().c_str()))
        {
            for (int i = 0; i < shaders.size(); ++i)
            {
                const auto &shader = shaders[i];
                bool isSelected = (selectedShaderIndex == i);
                if(ImGui::Selectable(shader->GetName().c_str(), isSelected))
                {
                    selectedShaderIndex = i;
                    // 设置当前材质的着色器
                    if (materialAsset)
                    {
                        materialAsset->SetShader(shader->GetShader());
                    }
                }
            }
            ImGui::EndCombo();
        }

        auto material = materialAsset->GetMaterial();
        if (!material)
        {
            ImGui::Text("No Material associated with this asset.");
            return;
        }

        auto textures = material->GetAllTextureBindings();
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
                        Logger::Log("MeshRendererInspector: Texture updated for slot {} ", binding->slot);
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

            ImGui::PopID();
        }

        // 添加新纹理绑定按钮
        if (ImGui::Button("Add Texture Binding"))
        {
            // 简化实现：添加一个新的 diffuse 纹理绑定
            std::string newTextureName = "texture_" + std::to_string(textures.size());
            std::shared_ptr<TextureAsset> newTex;
            material->SetTexture(newTextureName, newTex, textures.size(), TextureType::DIFFUSE);
        }
    }
}
