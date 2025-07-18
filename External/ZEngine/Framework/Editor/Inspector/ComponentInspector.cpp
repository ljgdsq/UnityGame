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
        if (!obj || !HasComponent(obj))
            return false;

        // 使用 GameObject 的泛型 RemoveComponent 方法
        // 这需要类型信息，我们通过类型索引来实现
        std::type_index typeIndex = GetComponentTypeIndex();

        // 这里我们需要一个通过 type_index 删除组件的方法
        // 但 GameObject 目前只有模板方法，我们需要添加一个基于 type_index 的删除方法
        // 或者让子类重写这个方法
        return false; // 暂时返回 false，需要在 GameObject 中添加支持
    }

    bool ComponentInspector::HasComponent(framework::GameObject *obj) const
    {
        return GetComponent(obj) != nullptr;
    }

    framework::Component *ComponentInspector::GetComponent(framework::GameObject *obj) const
    {
        // 基类实现返回 nullptr，子类应该重写这个方法
        return nullptr;
    }
}
