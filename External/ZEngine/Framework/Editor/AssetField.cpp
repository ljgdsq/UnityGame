#include "Framework/Editor/AssetField.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "imgui.h"

namespace editor
{

    // 通用默认实现
    template <typename AssetType>
    std::string AssetField<AssetType>::GetAssetTypeName()
    {
        return "Asset";
    }

    template <typename AssetType>
    framework::AssetType AssetField<AssetType>::GetAssetType()
    {
        return framework::AssetType::Unknown;
    }

    // TextureAsset 特化
    template <>
    std::string AssetField<framework::TextureAsset>::GetAssetTypeName()
    {
        return "Texture";
    }

    template <>
    framework::AssetType AssetField<framework::TextureAsset>::GetAssetType()
    {
        return framework::AssetType::Texture;
    }

    // MeshAsset 特化
    template <>
    std::string AssetField<framework::MeshAsset>::GetAssetTypeName()
    {
        return "Mesh";
    }

    template <>
    framework::AssetType AssetField<framework::MeshAsset>::GetAssetType()
    {
        return framework::AssetType::Mesh;
    }

    // TextureAsset UI 方法实现
    template <>
    bool AssetField<framework::TextureAsset>::RenderAssetSelector(
        const std::string &label,
        std::shared_ptr<framework::TextureAsset> &asset,
        const AssetFieldConfig &config)
    {
        // 简单实现：显示当前资源名称并提供选择按钮
        std::string assetName = asset ? asset->GetName() : "None";
        ImGui::Text("%s: %s", label.c_str(), assetName.c_str());

        ImGui::SameLine();
        if (ImGui::Button(("Select##" + label).c_str()))
        {
            // 这里可以实现资源选择对话框
            // 暂时返回 false 表示没有选择新资源
        }

        return false;
    }

    template <>
    bool AssetField<framework::TextureAsset>::RenderAssetButton(
        const std::string &label,
        std::shared_ptr<framework::TextureAsset> asset,
        const AssetFieldConfig &config)
    {
        std::string buttonText = asset ? asset->GetName() : "None";
        return ImGui::Button((buttonText + "##" + label).c_str());
    }

    template <>
    void AssetField<framework::TextureAsset>::RenderAssetThumbnail(
        std::shared_ptr<framework::TextureAsset> asset,
        const ImVec2 &size)
    {
        if (asset && asset->GetTexture() && asset->GetTexture()->GetTextureID() != 0)
        {
            ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(asset->GetTexture()->GetTextureID())), size);
        }
        else
        {
            // 绘制占位符
            ImGui::Dummy(size);
        }
    }

    // MeshAsset UI 方法实现
    template <>
    bool AssetField<framework::MeshAsset>::RenderAssetSelector(
        const std::string &label,
        std::shared_ptr<framework::MeshAsset> &asset,
        const AssetFieldConfig &config)
    {
        std::string assetName = asset ? asset->GetName() : "None";
        ImGui::Text("%s: %s", label.c_str(), assetName.c_str());

        ImGui::SameLine();
        if (ImGui::Button(("Select##" + label).c_str()))
        {
            // 这里可以实现资源选择对话框
            // 暂时返回 false 表示没有选择新资源
        }

        return false;
    }

    template <>
    bool AssetField<framework::MeshAsset>::RenderAssetButton(
        const std::string &label,
        std::shared_ptr<framework::MeshAsset> asset,
        const AssetFieldConfig &config)
    {
        std::string buttonText = asset ? asset->GetName() : "None";
        return ImGui::Button((buttonText + "##" + label).c_str());
    }

    template <>
    void AssetField<framework::MeshAsset>::RenderAssetThumbnail(
        std::shared_ptr<framework::MeshAsset> asset,
        const ImVec2 &size)
    {
        // Mesh 资源没有纹理，绘制占位符
        ImGui::Dummy(size);
        // 可以在这里绘制一个表示 3D 模型的图标
    }

    // 显式实例化常用类型
    template class AssetField<framework::TextureAsset>;
    template class AssetField<framework::MeshAsset>;

} // namespace editor
