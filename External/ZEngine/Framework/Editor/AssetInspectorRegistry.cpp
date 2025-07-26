#include "Framework/Editor/AssetInspectorRegistry.h"
#include "Framework/Editor/AssetEditorDefine.h"
#include "Framework/Editor/Inspector/AssetInspector.h"
#include "Framework/Log/Logger.h"
namespace editor
{

    REGISTER_ASSET_INSPECTOR(framework::AssetType::Unknown, AssetInspector);
    void AssetInspectorRegistry::RegisterInspector(AssetType assetType, AssetInspector *inspector)
    {
        // 检查是否已经存在相同类型的检查器
        if (HasInspector(assetType))
        {
            Logger::Warn("AssetInspectorRegistry: Inspector for asset type '{}' already exists. Overwriting.", AssetTypeToString(assetType));
            return; // 如果已经存在，则不再注册
        }
        if (inspector)
        {
            m_inspectors[assetType] = inspector;
        }
    }

    AssetInspector *AssetInspectorRegistry::GetInspector(framework::AssetType assetType) const
    {
        auto it = m_inspectors.find(assetType);
        if (it != m_inspectors.end())
        {
            // 如果找到了对应的检查器，返回它
            return it->second;
        }
        return m_inspectors.at(framework::AssetType::Unknown);
    }

    bool AssetInspectorRegistry::HasInspector(framework::AssetType assetType) const
    {
        return m_inspectors.find(assetType) != m_inspectors.end();
    }

}
