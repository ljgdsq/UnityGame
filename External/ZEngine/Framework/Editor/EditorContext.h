#pragma once
#include "Framework/Core/GameObject.h"
namespace editor{
    class EditorContext
    {
        private:
        static EditorContext *s_instance; // 单例实例
        public:
        // 获取单例实例
        static EditorContext *GetInstance()
        {
            if (!s_instance)
            {
                s_instance = new EditorContext();
            }
            return s_instance;
        }

    public:
        EditorContext() = default;
        ~EditorContext() = default;

        // 设置当前选中的游戏对象
        void SetSelectedGameObject(framework::GameObject *gameObject) { m_selectedGameObject = gameObject; }
        
        // 获取当前选中的游戏对象
        framework::GameObject *GetSelectedGameObject() const { return m_selectedGameObject; }

    private:
        framework::GameObject *m_selectedGameObject = nullptr; // 当前选中的游戏对象
    };
}