#pragma once
#include <memory>
#include <string>
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "imgui.h"

namespace editor
{

    struct AssetFieldConfig
    {
        bool showPreview = true;             // 显示预览图
        bool allowNull = true;               // 允许空值
        bool showClearButton = true;         // 显示清除按钮
        bool showSelectButton = true;        // 显示选择按钮
        ImVec2 previewSize = ImVec2(64, 64); // 预览图尺寸
        ImVec2 buttonSize = ImVec2(120, 20); // 按钮尺寸
        std::string nullText = "None";       // 空值显示文本
        std::string selectText = "Select";   // 选择按钮文本
        std::string clearText = "Clear";     // 清除按钮文本
    };

    // TextureAsset UI 工具函数
    bool RenderTextureField(const std::string &label, std::shared_ptr<framework::TextureAsset> &asset, const AssetFieldConfig &config);
    bool RenderTextureButton(const std::string &label, std::shared_ptr<framework::TextureAsset> asset, const AssetFieldConfig &config);
    void RenderTextureThumbnail(std::shared_ptr<framework::TextureAsset> asset, const ImVec2 &size);

    // MeshAsset UI 工具函数
    bool RenderMeshField(const std::string &label, std::shared_ptr<framework::MeshAsset> &asset, const AssetFieldConfig &config);
    bool RenderMeshButton(const std::string &label, std::shared_ptr<framework::MeshAsset> asset, const AssetFieldConfig &config);
    void RenderMeshThumbnail(std::shared_ptr<framework::MeshAsset> asset, const ImVec2 &size);
}
