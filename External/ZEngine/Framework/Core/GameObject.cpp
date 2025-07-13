#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "GameObject.h"

namespace framework
{
    GameObject::GameObject()
    {
        transform = AddComponent<Transform>();
    }

    GameObject::GameObject(std::string name)
    {
        transform = AddComponent<Transform>();
        this->name = name;
    }

    void GameObject::AddChild(GameObject *child)
    {
        if (child)
        {
            children.push_back(child);
            child->SetParent(this);
        }
    }

    void GameObject::RemoveChild(GameObject *child)
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

    void GameObject::OnCreate()
    {
    }

    void GameObject::OnInitialize()
    {
        // Initialize all components
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                component->OnInitialize();
            }
        }
    }

    void GameObject::OnDestroy()
    {
        // Destroy all components
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                component->OnDestroy();
                delete component; // Clean up memory
            }
            pair.second.clear();
        }
        components.clear();

        // Clear children
        for (auto *child : children)
        {
            child->SetParent(nullptr);
            delete child; // Clean up memory
        }
        children.clear();
    }
    void GameObject::SetActive(bool active)
    {
        isActive = active;
    }

} // namespace framework

