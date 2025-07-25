#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Asset/Asset.h"
using namespace framework;
namespace editor{
    class EditorContext
    {
        SINGLETON_CLASS(EditorContext)

    public:


        // 设置当前选中的游戏对象
        void SetSelectedGameObject(GameObject *gameObject) { m_selectedGameObject = gameObject; }

        // 获取当前选中的游戏对象
        GameObject *GetSelectedGameObject() const { return m_selectedGameObject; }

        void OnFileSelected(const std::string &path);
        std::shared_ptr<Asset> GetSelectedAsset() const { return m_selectedAsset; }

        void ClearSelectedAsset()
        {
            m_selectedAsset.reset();
        }

        void ClearSelectedGameObject()
        {
            m_selectedGameObject = nullptr;
        }
    private:
        GameObject *m_selectedGameObject = nullptr; // 当前选中的游戏对象
        std::shared_ptr<Asset> m_selectedAsset; // 当前选中的资源
    };
}
