// GameObject.h
#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "Framework/Component/Component.h"

namespace framework {
    class Transform;

    class GameObject final {
    private:
        enum class State {
            None, // 未初始化
            Created, // 已创建但未启用
            Started, // 已开始
            Destroyed // 已销毁
        };

        State state = State::None; // 当前状态
        bool isActive = true; // 是否激活
        bool isStarted = false; // 是否已开始
    public:
        GameObject();

        GameObject(std::string name);

        virtual ~GameObject() = default;

        void Update(float deltaTime);

    public:
        void Start();

        // Destroy the GameObject
        void Destroy() ;

        // Check if the GameObject is active
        bool IsActive() const;

        // Set the active state of the GameObject
        void SetActive(bool active);

    private:
#pragma region "GameObject Lifecycle"
        void OnEnable();

        void OnDisable();

        void OnDestroy();

#pragma endregion

    public:
        // Get the name of the GameObject
        const std::string &GetName() const { return name; }
        // Set the name of the GameObject
        void SetName(const std::string &name) { this->name = name; }

#pragma region "Component Management"

        // Add a component to the GameObject
        template<typename T>
        T *AddComponent();

        // Get a component of type T from the GameObject
        template<typename T>
        T *GetComponent() const;

        template<typename T>
        std::vector<T *> GetComponents() const;

        template<typename T>
        std::vector<T *> GetComponentsOfType() const;

        // Check if the GameObject has a component of type T
        template<typename T>
        bool HasComponent() const;

        // Remove a component of type T from the GameObject
        template<typename T>
        void RemoveComponent();

        // Get all components of type T in this GameObject and its children
        template<typename T>
        std::vector<T *> GetComponentsInChildren(bool includeInactive = false) const;

#pragma endregion

#pragma region "Child Management"

        // Add a child GameObject
        void AddChild(GameObject *child);

        // Remove a child GameObject
        void RemoveChild(GameObject *child);

        // Get the list of child GameObjects
        const std::vector<GameObject *> &GetChildren() const { return children; }

        // Get the parent GameObject
        GameObject *GetParent() const { return parent; }

        // Set the parent GameObject
        void SetParent(GameObject *parent) { this->parent = parent; }

#pragma endregion

        Transform *GetTransform() const { return transform; }

    private:
        std::string name;
        Transform *transform = nullptr; // Pointer to the Transform component

        std::unordered_map<std::type_index, std::vector<Component *> > components; // Map of components by type
        std::vector<Component *> m_pendingStartComponents; // Components waiting for Start call
        // ready to remove components
        std::vector<Component *> componentsToRemove;

        std::vector<GameObject *> children; // List of child GameObjects
        GameObject *parent = nullptr; // Pointer to the parent GameObject
    };

    // =============================================

    template<typename T>
    bool GameObject::HasComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "HasComponent T must be a subclass of Component");
        auto typeIndex = std::type_index(typeid(T));
        return components.find(typeIndex) != components.end() && !components.at(typeIndex).empty();
    }

    template<typename T>
    T *GameObject::AddComponent() {
        static_assert(std::is_base_of<Component, T>::value, "AddComponent T must be a subclass of Component");
        auto typeIndex = std::type_index(typeid(T));
        if (components.find(typeIndex) == components.end()) {
            components[typeIndex] = std::vector<Component *>();
        }
        T *component = new T(this);
        components[typeIndex].push_back(component);

        // 立即调用OnCreate
        component->OnCreate();

        // 根据GameObject状态决定后续调用
        if (isStarted) {
            // GameObject已经Started，将Component加入等待列表，下一帧调用OnStart
            m_pendingStartComponents.push_back(component);
        }
        // 如果GameObject还未Started，等GameObject.Start()时统一调用所有Component的OnStart

        return component;
    }

    template<typename T>
    T *GameObject::GetComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "GetComponent T must be a subclass of Component");
        auto typeIndex = std::type_index(typeid(T));
        auto it = components.find(typeIndex);
        if (it != components.end() && !it->second.empty()) {
            return static_cast<T *>(it->second.front());
        }
        return nullptr;
    }

    template<typename T>
    std::vector<T *> GameObject::GetComponents() const {
        static_assert(std::is_base_of<Component, T>::value, "GetComponents T must be a subclass of Component");
        auto typeIndex = std::type_index(typeid(T));
        auto it = components.find(typeIndex);
        if (it != components.end()) {
            std::vector<T *> result;
            for (auto component: it->second) {
                result.push_back(static_cast<T *>(component));
            }
            return result;
        }
        return {};
    }

    template<typename T>
    std::vector<T *> GameObject::GetComponentsOfType() const {
        static_assert(std::is_base_of<Component, T>::value, "GetComponentsOfType T must be a subclass of Component");
        std::vector<T *> result;

        // 遍历所有组件类型
        for (const auto &pair: components) {
            for (auto component: pair.second) {
                // 尝试动态转换为目标类型
                T *converted = dynamic_cast<T *>(component);
                if (converted) {
                    result.push_back(converted);
                }
            }
        }

        return result;
    }

    template<typename T>
    void GameObject::RemoveComponent() {
        static_assert(std::is_base_of<Component, T>::value, "RemoveComponent T must be a subclass of Component");
        auto typeIndex = std::type_index(typeid(T));
        auto it = components.find(typeIndex);
        if (it != components.end()) {
            for (auto component: it->second) {
                if (component) {
                    if (component->state == Component::State::Enabled || component->state == Component::State::Started
                        || component->state == Component::State::Updating) {
                        component->OnDisable();
                    }
                    component->OnDestroy();
                    delete component;
                }
            }
            components.erase(it);
        }

        m_pendingStartComponents.erase(
            std::remove_if(m_pendingStartComponents.begin(), m_pendingStartComponents.end(),
                           [typeIndex](Component *comp) { return std::type_index(typeid(*comp)) == typeIndex; }),
            m_pendingStartComponents.end());
    }

    template<typename T>
    std::vector<T *> GameObject::GetComponentsInChildren(bool includeInactive) const {
        static_assert(std::is_base_of<Component, T>::value,
                      "GetComponentsInChildren T must be a subclass of Component");
        std::vector<T *> result;

        // Check this GameObject's components
        auto componentsInThis = GetComponents<T>();
        result.insert(result.end(), componentsInThis.begin(), componentsInThis.end());

        // Recursively check children
        for (const auto &child: children) {
            if (includeInactive || child->IsActive()) {
                auto childComponents = child->GetComponentsInChildren<T>(includeInactive);
                result.insert(result.end(), childComponents.begin(), childComponents.end());
            }
        }

        return result;
    }
} // namespace framework
