#include "Framework/Core/Scene.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Render/RenderComponent.h"
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
                auto components = gameObject->GetComponents<RenderComponent>();
                renderComponents.insert(renderComponents.end(), components.begin(), components.end());
            }
        }
    }

    void Scene::SortRenderComponents( std::vector<RenderComponent *> &renderComponents)
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

    void Scene::Render(Renderer* renderer)
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

}