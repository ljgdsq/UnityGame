#pragma once
#include <string>
#include "imgui.h"
namespace editor
{
    class EditorWidget
    {
    public:
        // 构造函数
        EditorWidget(const std::string& name)
            : name(name) {}

        virtual ~EditorWidget() = default;

        // 初始化编辑器小部件
        virtual void Initialize() = 0;

        // 更新编辑器小部件
        virtual void Update(float deltaTime) = 0;

        // 渲染编辑器小部件
        virtual void Render() = 0;

        // 销毁编辑器小部件
        virtual void Shutdown() = 0;

    protected:
        // 编辑器小部件的名称
        std::string name;
    };
} // namespace editor
