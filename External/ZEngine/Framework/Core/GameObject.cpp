#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "GameObject.h"
#include "Framework/Core/SceneManager.h
namespace framework
{
    GameObject::GameObject()
    {
        transform = AddComponent<Transform>();
        SceneManager::GetInstance().AddGameObject(this);
    }

    GameObject::GameObject(std::string name)
    {
        transform = AddComponent<Transform>();
        this->name = name;
        SceneManager::GetInstance().AddGameObject(this);
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
        if (state == State::None)
        {
            state = State::Created;
        }
        else
        {
            Logger::Error("GameObject already created or in an invalid state.");
        }

    }

    void GameObject::OnStart()
    {
        if (state == State::Created)
        {
            state = State::Started;
            for (auto &pair : components)
            {
                for (auto *component : pair.second)
                {
                    component->OnStart();
                }
            }
        }
        else
        {
            Logger::Error("GameObject must be created before starting.");
        }
    }

    void GameObject::OnEnable()
    {
        if (state == State::Created || state == State::Disabled)
        {
            state = State::Enabled;
            for (auto &pair : components)
            {
                for (auto *component : pair.second)
                {
                    component->OnEnable();
                }
            }
        }
        else
        {
            Logger::Error("GameObject must be created or disabled before enabling.");
        }
    }

    void GameObject::OnDisable()
    {
        if (state == State::Enabled)
        {
            state = State::Disabled;
            for (auto &pair : components)
            {
                for (auto *component : pair.second)
                {
                    component->OnDisable();
                }
            }
        }
        else
        {
            Logger::Error("GameObject must be enabled before disabling.");
        }
    }

    void GameObject::OnDestroy()
    {
        if (state != State::Destroyed)
        {
            state = State::Destroyed;
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
        else
        {
            Logger::Error("GameObject already destroyed.");
        }
    }

    void GameObject::Update(float deltaTime)
    {
        if (state == State::Created)
        {
            /* code */
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

