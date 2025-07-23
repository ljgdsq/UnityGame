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
}
