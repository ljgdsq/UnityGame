#include "Framework/Editor/EditorContext.h"
#include "Framework/Asset/AssetManager.h"
namespace editor
{
    void EditorContext::OnFileSelected(const std::string &path)
    {
        ClearSelectedGameObject();
        ClearSelectedAsset();
        // 根据文件路径获取资源
        auto asset = AssetManager::GetInstance().LoadAsset<Asset>(path);
        if (asset && asset->IsLoaded())
        {
            m_selectedAsset = asset;
            Logger::Log("Selected asset: {}", asset->GetName());
        }
    }
}
