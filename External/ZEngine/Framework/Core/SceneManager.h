#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Common/Define.h"
#include <unordered_map>
#include <string>

namespace framework
{
    class Renderer; // 前向声明渲染器
    class SceneManager
    {

        SINGLETON_CLASS(SceneManager);

    private:
        std::unordered_map<std::string, Scene*> m_scenes;
        Scene* m_activeScene = nullptr;
        std::string m_activeSceneName;

    public:
        // 添加场景
        void AddScene(const std::string &name, Scene* scene);

        // 获取场景
        Scene* GetScene(const std::string &name);

        // 设置活动场景
        bool SetActiveScene(const std::string &name);

        // 获取活动场景
        Scene* GetActiveScene() const;

        // 获取活动场景名称
        const std::string &GetActiveSceneName() const;

        // 初始化活动场景
        void InitializeActiveScene();

        // 更新活动场景
        void UpdateActiveScene(float deltaTime);

        // 渲染活动场景
        void RenderActiveScene(Renderer *renderer);

        // 关闭活动场景
        void ShutdownActiveScene();
    };

} // namespace framework
