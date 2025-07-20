#include "Framework/Core/Scene.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Graphic/RenderComponent.h"
#include <algorithm>
namespace framework
{
    void Scene::CollectRenderComponents(std::vector<RenderComponent *> &renderComponents)
    {
        renderComponents.clear(); // 清空之前的渲染组件列表
        // 遍历所有游戏对象，收集它们的渲染组件
        for (auto &gameObject : m_gameObjects)
        {
            if (gameObject)
            {
                // 这里只会找到确切的 RenderComponent 类型，不包括子类
                auto components = gameObject->GetComponentsOfType<RenderComponent>();
                renderComponents.insert(renderComponents.end(), components.begin(), components.end());
            }
        }
    }

    void Scene::SortRenderComponents(std::vector<RenderComponent *> &renderComponents)
    {
        // 按渲染层级和排序顺序进行排序
        std::sort(renderComponents.begin(), renderComponents.end(),
                  [](const RenderComponent *a, const RenderComponent *b)
                  {
                      // 首先按渲染层级排序
                      if (a->GetRenderLayer() != b->GetRenderLayer())
                      {
                          return a->GetRenderLayer() < b->GetRenderLayer();
                      }
                      // 然后按排序顺序排序
                      return a->GetSortingOrder() < b->GetSortingOrder();
                  });
    }

    void Scene::Render(Renderer *renderer)
    {
        static std::vector<RenderComponent *> renderComponents;
        CollectRenderComponents(renderComponents);

        // 对收集到的渲染组件进行排序
        SortRenderComponents(renderComponents);

        // 遍历排序后的渲染组件并调用它们的渲染方法
        for (auto &component : renderComponents)
        {
            if (component && component->IsVisible())
            {
                component->Render(renderer);
            }
        }
    }

    void Scene::AddGameObject(GameObject *gameObject)
    {
        if (!gameObject)
        {
            Logger::Error("Attempted to add a null GameObject to the scene.");
            return;
        }

        if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) != m_gameObjects.end())
        {
            Logger::Warn("GameObject already exists in the scene.");
            return; // 如果游戏对象已经存在，直接返回
        }

        if (gameObject)
        {
            m_gameObjects.push_back(gameObject);
            m_newGameObjects.push_back(gameObject);
        }
        else
        {
            Logger::Error("Attempted to add a null GameObject to the scene.");
        }
    }

    void Scene::RemoveGameObject(GameObject *gameObject)
    {
        if (gameObject)
        {
            auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
            if (it != m_gameObjects.end())
            {
                gameObject->Destroy(); // 调用销毁时的回调
                m_gameObjects.erase(it); // 从场景中移除游戏对象
            }
            else
            {
                Logger::Warn("GameObject not found in the scene.");
            }

            // 如果游戏对象在新添加列表中，也需要移除
            auto newIt = std::find(m_newGameObjects.begin(), m_newGameObjects.end
            (), gameObject);
            if (newIt != m_newGameObjects.end())
            {
                m_newGameObjects.erase(newIt);
            }
        }
        else
        {
            Logger::Error("Attempted to remove a null GameObject from the scene.");
        }
    }

    void Scene::Update(float deltaTime)
    {
        // 遍历新添加的游戏对象并调用它们的 OnCreate 方法
        for (auto &gameObject : m_newGameObjects)
        {
            if (gameObject)
            {
                gameObject->Start(); // 调用 Start 方法
            }
        }
        m_newGameObjects.clear(); // 清空新添加的游戏对象列表

        // 遍历所有游戏对象并更新它们
        for (auto &gameObject : m_gameObjects)
        {
            if (!gameObject)
                continue; // 如果游戏对象为空，跳过
            gameObject->Update(deltaTime);
        }
    }

}
