#pragma once
#include "imgui.h"
#include <string>
#include <typeindex>
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "Framework/Component/Component.h"

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

        // 获取组件显示名称 - 每个检查器需要实现
        virtual std::string GetComponentDisplayName() const = 0;

        // 获取组件类型信息 - 每个检查器需要实现
        virtual std::type_index GetComponentTypeIndex() const = 0;

        // 通用方法 - 基于类型信息实现
        bool IsComponentEnabled(framework::GameObject *obj) const;
        void SetComponentEnabled(framework::GameObject *obj, bool enabled);
        bool RemoveComponent(framework::GameObject *obj);
        bool HasComponent(framework::GameObject *obj) const;

    protected:
        // 获取具体类型的组件指针 - 子类可以重写以提供类型安全的访问
        virtual framework::Component *GetComponent(framework::GameObject *obj) const;
    };

    // 模板基类，简化具体检查器的实现
    template <typename ComponentType>
    class TypedComponentInspector : public ComponentInspector
    {
    public:
        std::type_index GetComponentTypeIndex() const override
        {
            return std::type_index(typeid(ComponentType));
        }

    protected:
        ComponentType *GetTypedComponent(framework::GameObject *obj) const
        {
            if (!obj || !obj->HasComponent<ComponentType>())
                return nullptr;
            return obj->GetComponent<ComponentType>();
        }

        framework::Component *GetComponent(framework::GameObject *obj) const override
        {
            return GetTypedComponent(obj);
        }
    };
}
