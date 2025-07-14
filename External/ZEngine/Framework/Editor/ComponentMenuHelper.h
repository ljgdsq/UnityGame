#pragma once
#include "imgui.h"

namespace framework
{
    class GameObject;
}

namespace editor
{

    class ComponentMenuHelper
    {
    public:
        // 打开添加组件菜单
        static void OpenAddComponentMenu(framework::GameObject *gameObject);

        // 渲染添加组件菜单
        static void RenderAddComponentMenu();

    private:
        static bool s_showAddComponentPopup;
        static framework::GameObject *s_targetGameObject;
    };
}
