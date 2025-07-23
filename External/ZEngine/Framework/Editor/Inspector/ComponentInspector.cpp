#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Component/Component.h"

namespace editor
{
    bool ComponentInspector::IsComponentEnabled(framework::GameObject *obj) const
    {
        framework::Component *component = GetComponent(obj);
        return component && component->IsActive();
    }

    void ComponentInspector::SetComponentEnabled(framework::GameObject *obj, bool enabled)
    {
        framework::Component *component = GetComponent(obj);
        if (component)
            component->SetEnabled(enabled);
    }

    bool ComponentInspector::RemoveComponent(framework::GameObject *obj)
    {
        return RemoveComponentT(obj);
    }

    bool ComponentInspector::HasComponent(framework::GameObject *obj) const
    {
        return GetComponent(obj) != nullptr;
    }

    // 基类的默认实现（通常会被子类重写）
    framework::Component *ComponentInspector::GetComponent(framework::GameObject *obj) const
    {
        // 基类无法知道具体的组件类型，返回nullptr
        // 子类应该重写这个方法
        return nullptr;
    }

    bool ComponentInspector::RemoveComponentT(framework::GameObject *obj)
    {
        // 基类无法知道具体的组件类型，返回false
        // 子类应该重写这个方法
        return false;
    }
}
