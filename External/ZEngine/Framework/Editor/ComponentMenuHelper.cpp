#include "Framework/Editor/ComponentMenuHelper.h"
#include "Framework/Component/ComponentRegistry.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Log/Logger.h"

namespace editor
{
    bool ComponentMenuHelper::s_showAddComponentPopup = false;
    framework::GameObject *ComponentMenuHelper::s_targetGameObject = nullptr;

    void ComponentMenuHelper::OpenAddComponentMenu(framework::GameObject *gameObject)
    {
        s_targetGameObject = gameObject;
        s_showAddComponentPopup = true;
    }

    void ComponentMenuHelper::RenderAddComponentMenu()
    {
        if (s_showAddComponentPopup)
        {
            ImGui::OpenPopup("AddComponentPopup");
            s_showAddComponentPopup = false;
        }

        // Add Component 弹窗菜单
        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            if (s_targetGameObject)
            {
                auto &registry = framework::ComponentRegistry::GetInstance();
                const auto &components = registry.GetAllComponents();

                ImGui::Text("Select Component to Add:");
                ImGui::Separator();

                // 列出所有注册的组件
                for (const auto &comp : components)
                {
                    if (ImGui::MenuItem(comp.name.c_str()))
                    {
                        // 创建并添加组件
                        auto *newComponent = registry.CreateComponent(comp.name, s_targetGameObject);
                        if (newComponent)
                        {
                            Logger::Log("Successfully added component: {} to GameObject: {}",
                                        comp.name, s_targetGameObject->GetName());
                        }
                        else
                        {
                            Logger::Error("Failed to add component: {} to GameObject: {}",
                                          comp.name, s_targetGameObject->GetName());
                        }
                        ImGui::CloseCurrentPopup();
                    }
                }

                if (components.empty())
                {
                    ImGui::Text("No components registered.");
                    ImGui::Text("Call RegisterAllComponents() to register components.");
                }
            }
            else
            {
                ImGui::Text("No GameObject selected.");
            }

            ImGui::EndPopup();
        }
    }
}
