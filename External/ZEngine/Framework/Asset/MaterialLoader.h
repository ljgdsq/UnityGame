#pragma once
#include "Framework/Asset/AssetLoader.h"

namespace framework
{
    class MaterialAsset;

    /**
     * @brief 着色器加载器
     * 用于加载材质资源
     */
    class MaterialLoader : public AssetLoader
    {
    public:
        MaterialLoader() = default;
        virtual ~MaterialLoader() = default;

        // 加载着色器资源
        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) override;

        // 获取支持的扩展名列表
        std::vector<std::string> GetSupportedExtensions() const override;

        // 获取加载器名称
        std::string GetName() const override { return "MaterialLoader"; }

        // 获取资源类型
        AssetType GetAssetType() const override { return AssetType::Material; }

    private:
        std::shared_ptr<MaterialAsset> m_materialAsset; // 缓存材质资源
    };
} // namespace framework
