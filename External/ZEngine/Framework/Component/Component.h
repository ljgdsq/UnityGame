// Component.h
#pragma once
#include <string>
#include <typeinfo>
#include "Framework/Core/Serializable.h"
#include "Framework/Log/Logger.h"

namespace framework {
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
    virtual rapidjson::Value Serialize() const override = 0;


    virtual void Deserialize(const rapidjson::Value& jsonValue) override = 0;

    bool IsEnabled() const { return isEnabled; }    
    void SetEnabled(bool enabled) { isEnabled = enabled; }  

    // type info
    virtual const char *GetTypeName() const = 0;
    virtual const std::type_info &GetTypeInfo() const = 0;

protected:
    virtual void OnCreate() {};
    virtual void OnInitialize() {};
    virtual void OnEnable() {};
    virtual void OnStart() {};
    virtual void OnUpdate(float deltaTime){};
    virtual void OnDisable() {};
    virtual void OnDestroy() {};


private:
    GameObject *gameObject; // Pointer to the GameObject this component is attached to
    bool isEnabled = true; // Whether the component is enabled
};

// Base class for all components 
template<typename T>
class ComponentBase : public Component
{
    public:
    explicit ComponentBase(GameObject *gameObject) : Component(gameObject) {}
    
    // serialization
    virtual ~ComponentBase() = default;
    virtual rapidjson::Value Serialize() const override = 0;
    virtual void Deserialize(const rapidjson::Value& jsonValue) override = 0;

    // Returns the name of the component    
    virtual const char *GetName() const override { return GetTypeName(); }

    // type info
    virtual const char *GetTypeName() const override { return typeid(T).name(); }
    virtual const std::type_info &GetTypeInfo() const override { return typeid(T); }
};
}
