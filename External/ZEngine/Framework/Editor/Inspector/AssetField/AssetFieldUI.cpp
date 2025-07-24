#include "AssetFieldUI.h"
#include "Framework/Editor/AssetDragDropSystem.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Log/Logger.h"
namespace editor
{
    // TextureAsset UI 工具函数实现
    bool RenderTextureField(const std::string &label, std::shared_ptr<framework::TextureAsset> &asset, const AssetFieldConfig &config)
    {

        std::string assetName = asset ? asset->GetName() : config.nullText;

        ImGui::PushID(label.c_str());
        ImGui::Text("%s", label.c_str());

        ImGui::Text("Asset: %s", assetName.c_str());
        bool changed = false;
        if (config.showPreview)
        {
            // 保存当前光标位置
            ImVec2 previewPos = ImGui::GetCursorPos();

            // 渲染预览图
            RenderTextureThumbnail(asset, config.previewSize);

            DragDropPayload payload;

            if (AssetDragDropSystem::AcceptDragDrop(DragDropType::Texture, payload))
            {
                if (payload.IsValid())
                {
                    auto textureAsset = framework::AssetManager::GetInstance().GetAsset<framework::TextureAsset>(payload.dataId);
                    if (textureAsset)
                    {
                        asset = textureAsset;
                        changed = true;
                    }
                }
            }
        }

        if (config.showClearButton)
        {
            ImGui::SameLine();
            if (ImGui::Button((config.clearText + "##" + label).c_str()))
            {
                asset.reset();
                ImGui::PopID();
                return true;
            }
        }
        ImGui::PopID();
        return changed;
    }

    bool RenderTextureButton(const std::string &label, std::shared_ptr<framework::TextureAsset> asset, const AssetFieldConfig &config)
    {
        std::string buttonText = asset ? asset->GetName() : config.nullText;
        return ImGui::Button((buttonText + "##" + label).c_str(), config.buttonSize);
    }

    void RenderTextureThumbnail(std::shared_ptr<framework::TextureAsset> asset, const ImVec2 &size)
    {
        if (asset && asset->GetTexture() && asset->GetTexture()->GetTextureID() != 0)
        {
            ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(asset->GetTexture()->GetTextureID())), size);
        }
        else
        {
            ImGui::Dummy(size);
        }
    }

    bool RenderMeshField(const std::string &label, std::shared_ptr<framework::MeshAsset> &asset, const AssetFieldConfig &config)
    {
        ImGui::PushID(label.c_str());
        ImGui::Text("%s", label.c_str());
        if (config.showPreview)
        {
            ImGui::SameLine();
            RenderMeshThumbnail(asset, config.previewSize);
        }
        ImGui::SameLine();
        std::string assetName = asset ? asset->GetName() : config.nullText;
        if (ImGui::Button((assetName + "##" + label).c_str(), config.buttonSize))
        {
            // 这里可以实现资源选择对话框
        }
        if (config.showClearButton)
        {
            ImGui::SameLine();
            if (ImGui::Button((config.clearText + "##" + label).c_str()))
            {
                asset.reset();
                ImGui::PopID();
                return true;
            }
        }
        ImGui::PopID();
        return false;
    }

    bool RenderMeshButton(const std::string &label, std::shared_ptr<framework::MeshAsset> asset, const AssetFieldConfig &config)
    {
        std::string buttonText = asset ? asset->GetName() : config.nullText;
        return ImGui::Button((buttonText + "##" + label).c_str(), config.buttonSize);
    }

    void RenderMeshThumbnail(std::shared_ptr<framework::MeshAsset> asset, const ImVec2 &size)
    {
        ImGui::Dummy(size);
        // 可以在这里绘制一个表示 3D 模型的图标
    }
}
