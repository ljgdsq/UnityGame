#pragma once
#include "Framework/Editor/EditorWidget.h"
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Asset/Asset.h"
#include <vector>
namespace editor
{
    class Inspector : public EditorWidget
    {
    public:
        Inspector();
        ~Inspector() override = default;

        // 初始化检查器小部件
        void Initialize() override;

        // 更新检查器小部件
        void Update(float deltaTime) override;

        // 渲染检查器小部件
        void Render() override;

        // 销毁检查器小部件
        void Shutdown() override;

    public:
        // 设置当前选中的游戏对象
        void SetSelectedGameObject(framework::GameObject *gameObject);
        void SetSelectedAsset(std::shared_ptr<framework::Asset> asset);

    private:
        // 当前选中的游戏对象
        framework::GameObject *m_selectedGameObject = nullptr;
        std::shared_ptr<framework::Asset> m_selectedAsset;
        // 渲染检查器内容
        void RenderInspectorContent();

        // 新增：渲染单个组件检查器
        void RenderComponentInspector(ComponentInspector *inspector, size_t index);

        // 新增：渲染组件头部（折叠箭头 + 启用切换 + 组件名称）
        bool RenderComponentHeader(ComponentInspector *inspector, bool &enabled);

        // 新增：渲染组件操作按钮（更多菜单 + 删除按钮）
        void RenderComponentActions(ComponentInspector *inspector);

        // 新增：检查对象是否有特定组件
        bool HasComponent(ComponentInspector *inspector, framework::GameObject *obj);

        std::vector<ComponentInspector *> m_inspectors; // 存储所有组件检查器
    };

}
