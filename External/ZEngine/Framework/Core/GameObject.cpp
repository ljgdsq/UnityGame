#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/Scene.h"
#include "Framework/Component/ComponentRegistry.h"
namespace framework
{
    GameObject::GameObject(Scene *scene)
    {
        transform = AddComponent<Transform>();
        if (scene == nullptr)
        {
            SceneManager::GetInstance().GetActiveScene()->AddGameObject(this);
        }
        else
        {
            scene->AddGameObject(this);
        }
    }

    GameObject::GameObject(std::string name, Scene *scene)
    {
        transform = AddComponent<Transform>();
        this->name = name;
        if (scene == nullptr)
        {
            SceneManager::GetInstance().GetActiveScene()->AddGameObject(this);
        }
        else
        {
            scene->AddGameObject(this);
        }
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

    bool GameObject::IsActive() const
    {
        return isActive;
    }

    void GameObject::Destroy()
    {
        if (state == State::Destroyed)
        {
            Logger::Error("GameObject already destroyed.");
            return;
        }
        state = State::Destroyed;

        // 调用销毁时的回调
        OnDestroy();

        // 清理组件
        for (auto &pair : components)
        {
            for (auto *component : pair.second)
            {
                delete component; // 释放内存
            }
            pair.second.clear();
        }
        components.clear();

        // 清理子对象
        for (auto *child : children)
        {
            child->SetParent(nullptr);
            delete child; // 释放内存
        }
        children.clear();
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
        // todo : process children
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

    rapidjson::Value GameObject::Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const
    {
        rapidjson::Value jsonObject(rapidjson::kObjectType);
        jsonObject.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
        jsonObject.AddMember("isActive", isActive, allocator);

        // 序列化所有组件
        rapidjson::Value componentsArray(rapidjson::kArrayType);
        for (const auto &pair : components)
        {
            for (const auto *component : pair.second)
            {
                rapidjson::Value componentJson = component->Serialize(allocator);
                componentsArray.PushBack(componentJson, allocator);
            }
        }
        jsonObject.AddMember("components", componentsArray, allocator);

        return jsonObject;
    }

    void GameObject::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (!jsonValue.IsObject())
        {
            Logger::Error("Invalid JSON value for GameObject deserialization.");
            return;
        }

        if (jsonValue.HasMember("name") && jsonValue["name"].IsString())
        {
            name = jsonValue["name"].GetString();
        }

        if (jsonValue.HasMember("isActive") && jsonValue["isActive"].IsBool())
        {
            isActive = jsonValue["isActive"].GetBool();
        }

        if (jsonValue.HasMember("components") && jsonValue["components"].IsArray())
        {
            const rapidjson::Value &componentsArray = jsonValue["components"];
            if (componentsArray.IsArray() && componentsArray.Size() > 0)
            {
                for (rapidjson::SizeType i = 0; i < componentsArray.Size(); ++i)
                {
                    const rapidjson::Value &componentValue = componentsArray[i];
                    if (componentValue.IsObject() && componentValue.HasMember("type"))
                    {
                        std::string typeName = componentValue["type"].GetString();

                        Component *component = ComponentRegistry::GetInstance().CreateComponent(typeName, this);
                        if (component)
                        {
                            component->Deserialize(componentValue);
                        }
                        else
                        {
                            Logger::Warn("Unknown component type: {}", typeName);
                        }
                    }
                }
            }
        }
    }

} // namespace framework
