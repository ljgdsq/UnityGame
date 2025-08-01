// Component.h
#pragma once
#include <string>
#include <typeinfo>
#include "glm/glm.hpp"
#include "Framework/Core/Serializable.h"
#include "Framework/Log/Logger.h"

namespace framework
{
    class GameObject;

    // Base class for all components (not intended to be instantiated directly)
    class Component : public Serializable
    {
        friend class GameObject; // Allow GameObject to access
    public:
        explicit Component(GameObject *gameObject) : gameObject(gameObject) {}

    public:
        virtual ~Component() = default;

        // Returns the GameObject this component is attached to
        virtual GameObject *GetGameObject() const { return gameObject; }

        // Returns the name of the component
        virtual const char *GetName() const { return GetTypeName(); }

        // Serializes the component to a JSON value
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator ) const override = 0;
        // Deserializes the component from a JSON value
        virtual void Deserialize(const rapidjson::Value &jsonValue) override = 0;

        // type info
        virtual const char *GetTypeName() const = 0;
        virtual const std::type_info &GetTypeInfo() const = 0;

        virtual void OnComponentAdd(Component *component) {}
    protected:
        virtual void OnCreate() ;
        virtual void OnEnable() ;
        virtual void OnStart() ;
        virtual void OnUpdate(float deltaTime);
        virtual void OnDisable();
        virtual void OnDestroy();

    protected:
        GameObject *gameObject; // Pointer to the GameObject this component is attached to
        bool isActive = true;   // Whether the component is enabled

    protected:
        enum class State
        {
            None,     // 未初始化
            Created,  // 已创建但未启用
            Enabled,  // 已启用
            Started,  // 已开始
            Updating, // 正在更新
            Disabled, // 已禁用
            Destroyed // 已销毁
        };
        State state = State::None; // 当前状态
    public:
        bool IsActive() const { return isActive; }
        void SetEnabled(bool enabled);
        bool CanUpdate() const { return isActive && (state == State::Enabled || state == State::Updating); }
        void Destroy();
    };

    // Base class for all components
    template <typename T>
    class ComponentBase : public Component
    {
    public:
        explicit ComponentBase(GameObject *gameObject) : Component(gameObject) {}

        // serialization
        virtual ~ComponentBase() = default;
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override = 0;
        virtual void Deserialize(const rapidjson::Value &jsonValue) override = 0;

        // Returns the name of the component
        virtual const char *GetName() const override { return GetTypeName(); }

        // type info
        virtual const char *GetTypeName() const override { return typeid(T).name(); }
        virtual const std::type_info &GetTypeInfo() const override { return typeid(T); }
    };
}
