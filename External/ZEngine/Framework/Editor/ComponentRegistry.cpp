#include "Framework/Editor/ComponentRegistry.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Log/Logger.h"

namespace framework
{

    ComponentRegistry &ComponentRegistry::GetInstance()
    {
        static ComponentRegistry instance;
        return instance;
    }

    const std::vector<ComponentInfo> &ComponentRegistry::GetAllComponents() const
    {
        return m_components;
    }

    Component *ComponentRegistry::CreateComponent(const std::string &name, GameObject *owner)
    {
        if (!owner)
        {
            Logger::Error("Cannot create component: GameObject is null");
            return nullptr;
        }

        auto it = m_nameToIndex.find(name);
        if (it != m_nameToIndex.end())
        {
            try
            {
                Component *component = m_components[it->second].creator(owner);
                Logger::Log("Created component: {} for GameObject: {}", name, owner->GetName());
                return component;
            }
            catch (const std::exception &e)
            {
                Logger::Error("Failed to create component {}: {}", name, e.what());
                return nullptr;
            }
        }

        Logger::Warn("Component not found in registry: {}", name);
        return nullptr;
    }
}
