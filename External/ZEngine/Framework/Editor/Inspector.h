#pragma once
#include "Framework/Editor/EditorWidget.h"
#include "Framework/Editor/Inspector/ComponentInspector.h"

#include <vector>
namespace editor {
    class Inspector : public EditorWidget {
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

        private:
            // 当前选中的游戏对象
            framework::GameObject* m_selectedGameObject = nullptr;

            // 渲染检查器内容
            void RenderInspectorContent();
            
            std::vector<ComponentInspector*> m_inspectors; // 存储所有组件检查器
        };


}