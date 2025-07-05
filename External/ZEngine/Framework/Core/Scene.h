#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Framework/Log/Logger.h"
namespace framework
{
    class GameObject;      // 前向声明，避免循环依赖
    class RenderComponent; // 前向声明渲染组件
    class Renderer;
    class Scene
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual void Initialize() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(Renderer *renderer);
        virtual void Shutdown() = 0;

        virtual const char *GetName() const = 0;
    protected:
        virtual void AddGameObject(GameObject *gameObject);
        virtual void RemoveGameObject(GameObject *gameObject);
  
    private:
        // 禁止拷贝和赋值
        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;
        std::vector<GameObject *> m_gameObjects; // 存储场景中的游戏对象

        void CollectRenderComponents(std::vector<RenderComponent *> &renderComponents);

        void SortRenderComponents(std::vector<RenderComponent *> &renderComponents);
    };

    using ScenePtr = std::shared_ptr<Scene>;

} // namespace framework
