#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace framework
{
    class GameObject;
    class Component;

    // 组件创建器类型
    using ComponentCreator = std::function<Component *(GameObject *)>;

    // 组件信息
    struct ComponentInfo
    {
        std::string name;         // 显示名称
        ComponentCreator creator; // 创建函数

        ComponentInfo(const std::string &n, ComponentCreator c)
            : name(n), creator(c) {}
    };

    // 简化的组件注册系统
    class ComponentRegistry
    {
    public:
        static ComponentRegistry &GetInstance();

        // 注册组件
        template <typename T>
        void RegisterComponent(const std::string &displayName);

        // 获取所有注册的组件
        const std::vector<ComponentInfo> &GetAllComponents() const;

        // 创建组件
        Component *CreateComponent(const std::string &name, GameObject *owner);

    private:
        ComponentRegistry() = default;
        std::vector<ComponentInfo> m_components;
        std::unordered_map<std::string, size_t> m_nameToIndex;
    };

    // 模板实现
    template <typename T>
    void ComponentRegistry::RegisterComponent(const std::string &displayName)
    {
        ComponentCreator creator = [](GameObject *owner) -> Component *
        {
            return owner->AddComponent<T>();
        };

        ComponentInfo info(displayName, creator);
        m_nameToIndex[displayName] = m_components.size();
        m_components.push_back(info);
    }
}
