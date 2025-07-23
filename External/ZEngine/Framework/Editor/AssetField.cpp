#include "Framework/Editor/AssetField.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"

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

    // 显式实例化常用类型
    template class AssetField<framework::TextureAsset>;
    template class AssetField<framework::MeshAsset>;

} // namespace editor
