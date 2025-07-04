#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
framework::GameObject::GameObject()
{
    transform = AddComponent<Transform>();
}

void framework::GameObject::AddChild(GameObject *child)
{
    if (child)
    {
        children.push_back(child);
        child->SetParent(this);
    }
}

void framework::GameObject::RemoveChild(GameObject *child)
{
    if (child)
    {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end())
        {
            children.erase(it);
            child->SetParent(nullptr);
        }
    }
}
