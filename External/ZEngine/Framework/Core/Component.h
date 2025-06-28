#pragma once
#include "Framework/Core/Serializable.h"

class GameObject;

class Component : public Serializable
{
public:
    explicit Component(GameObject *gameObject) : gameObject(gameObject) {}

public:
    virtual ~Component() = default;

    // Returns the GameObject this component is attached to
    virtual GameObject *GetGameObject() const = 0;

    // Returns the name of the component
    virtual const char *GetName() const = 0;

    // Serializes the component to a JSON value
    virtual rapidjson::Value Serialize() const override = 0;

    virtual void Deserialize() = 0;

    // Returns the type of the component as a string
    virtual const char *GetType() const = 0;

protected:
    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;
    virtual void OnStart() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;

private:
    GameObject *gameObject; // Pointer to the GameObject this component is attached to
};