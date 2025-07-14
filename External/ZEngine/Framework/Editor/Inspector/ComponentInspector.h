#pragma once
#include "imgui.h"
#include <string>
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
namespace editor
{
    class ComponentInspector
    {
    public:
        ComponentInspector() = default;
        virtual ~ComponentInspector() = default;
        virtual void Update(float deltaTime) {}

    public:
        virtual void Inspect(framework::GameObject *node) = 0;

        // 新增：获取组件显示名称
        virtual std::string GetComponentDisplayName() const = 0;

        // 新增：检查组件是否存在且启用
        virtual bool IsComponentEnabled(framework::GameObject *obj) const = 0;

        // 新增：设置组件启用状态
        virtual void SetComponentEnabled(framework::GameObject *obj, bool enabled) = 0;

        // 新增：删除组件
        virtual bool RemoveComponent(framework::GameObject *obj) = 0;
    };
}
