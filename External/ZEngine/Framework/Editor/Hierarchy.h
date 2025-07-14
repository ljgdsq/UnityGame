#pragma once
#include <vector>
#include "Framework/Editor/EditorWidget.h"
namespace editor
{
    class HierarchyWidget : public EditorWidget
    {
    public:
        HierarchyWidget();
        ~HierarchyWidget() override;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Shutdown() override;

    private:
        class ContextMenu *contextMenu;          // 选中物体的上下文菜单
        class ContextMenu *emptyAreaContextMenu; // 空白区域的上下文菜单
    };
} // namespace editor
