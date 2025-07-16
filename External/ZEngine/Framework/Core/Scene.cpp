#include "Framework/Core/Scene.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Graphic/RenderComponent.h"
#include <algorithm>
#include "Scene.h"
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
        if (gameObject)
        {
            m_gameObjects.push_back(gameObject);
            gameObject->SetActive(true); // 确保新添加的游戏对象是激活状态
            gameObject->OnCreate();      // 调用创建时的回调
            gameObject->OnInitialize();  // 调用初始化时的回调
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
                gameObject->OnDestroy(); // 调用销毁时的回调
                m_gameObjects.erase(it); // 从场景中移除游戏对象
            }
            else
            {
                Logger::Warn("GameObject not found in the scene.");
            }
        }
        else
        {
            Logger::Error("Attempted to remove a null GameObject from the scene.");
        }
    }

}
