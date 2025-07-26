#pragma once

#include "Framework/Common/Define.h"
#include "Framework/Asset/Asset.h"
#include "Framework/Editor/Inspector/AssetInspector.h"
#include <unordered_map>

namespace editor
{

    using framework::AssetType;
    /**
     * @brief 资源检查器注册表，用于管理所有资源检查器
     */
    class AssetInspectorRegistry
    {
        SINGLETON_CLASS(AssetInspectorRegistry);

    public:
        // 注册资源检查器
        void RegisterInspector(AssetType assetType, AssetInspector *inspector);

        // 获取资源检查器
        AssetInspector *GetInspector(AssetType assetType) const;

        // 检查是否存在指定类型的资源检查器
        bool HasInspector(AssetType assetType) const;

    private:
        std::unordered_map<AssetType, AssetInspector *> m_inspectors; // 存储所有注册的资源检查器
    };
} // namespace editor
