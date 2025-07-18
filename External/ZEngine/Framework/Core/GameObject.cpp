#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "Framework/Core/SceneManager.h"
namespace framework
{
    GameObject::GameObject()
    {
        transform = AddComponent<Transform>();
        SceneManager::GetInstance().GetActiveScene()->AddGameObject(this);
    }

    GameObject::GameObject(std::string name)
    {
        transform = AddComponent<Transform>();
        this->name = name;
        SceneManager::GetInstance().GetActiveScene()->AddGameObject(this);
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

    void GameObject::Start()
    {
        if (isStarted)
        {
            Logger::Warn("GameObject has already started.");
            return;
        }
        isStarted = true;
        state = State::Started;

        // 调用所有现有Component的OnStart
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                component->OnStart();
            }
        }

        if (isActive)
        {
            OnEnable();
        }
        else
        {
            OnDisable();
        }
    }

    void GameObject::OnEnable()
    {
        if (!isStarted)
        {
            Logger::Error("GameObject must be started before enabling.");
            return;
        }
        if (!isActive)
        {
            Logger::Warn("GameObject is not active, cannot enable.");
            return;
        }
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                if (component->IsActive())
                {
                    component->OnEnable();
                }
            }
        }
    }

    void GameObject::OnDisable()
    {
        if (!isStarted)
        {
            Logger::Error("GameObject must be started before disabling.");
            return;
        }
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                if (component->IsActive())
                {
                    component->OnDisable();
                }
            }
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
                    if (component->IsActive())
                    {
                        component->OnDisable();
                    }
                    component->OnDestroy();

                    componentsToRemove.push_back(component);
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
        if (!isActive)
            return;

        if (!componentsToRemove.empty())
        {
            for (auto *component : componentsToRemove)
            {
                delete component;
            }
            componentsToRemove.clear();
        }

        // 处理等待Start的组件
        if (!m_pendingStartComponents.empty())
        {
            for (auto *component : m_pendingStartComponents)
            {
                component->OnStart();
            }

            for (auto *component : m_pendingStartComponents)
            {
                if (component->IsActive())
                {
                    component->OnEnable();
                }
                else
                {
                    component->OnDisable();
                }
            }

            m_pendingStartComponents.clear();
        }

        // 更新所有启用的组件
        if (isStarted)
        {
            for (auto &pair : components)
            {
                for (auto *component : pair.second)
                {
                    if (component->CanUpdate())
                    {
                        component->OnUpdate(deltaTime);
                    }
                }
            }
        }
    }

    void GameObject::SetActive(bool active)
    {
        //todo : process children
        if (isActive == active)
            return; // 如果状态没有变化，直接返回
        isActive = active;
        if (!isStarted)
            return; // 如果还未开始，直接返回

        if (isActive)
        {
            OnEnable();
        }
        else
        {
            OnDisable();
        }
    }

} // namespace framework
