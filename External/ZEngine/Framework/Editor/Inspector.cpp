#include "Framework/Editor/Inspector.h"
#include "Framework/Editor/Inspector/TransformInspector.h"
#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Editor/Inspector/MeshFilterInspector.h"
#include "Framework/Editor/EditorContext.h"
#include "Framework/Editor/Inspector/LightInspector.h"
#include "Framework/Editor/ComponentMenuHelper.h"
#include <imgui_internal.h>
namespace editor
{

    Inspector::Inspector() : EditorWidget("Inspector"),
                             m_selectedGameObject(nullptr)
    {
        // 初始化组件检查器
        m_inspectors.push_back(new TransformInspector());
        // 可以在这里添加更多的组件检查器
        m_inspectors.push_back(new MeshRendererInspector());
        m_inspectors.push_back(new MeshFilterInspector());

        m_inspectors.push_back(new LightInspector()); // 添加光照检查器
    }

    void Inspector::Initialize()
    {
    }
    void Inspector::Update(float deltaTime)
    {

        // todo : EditorContext::GetInstance()->GetSelectedGameObject() remove?
        SetSelectedGameObject(EditorContext::GetInstance()->GetSelectedGameObject());

        for (auto inspector : m_inspectors)
        {
            inspector->Update(deltaTime);
        }
    }

    void Inspector::Render()
    {
        ImGui::Begin(name.c_str());
        if (m_selectedGameObject)
        {
            ImGui::Text("Selected GameObject: %s", m_selectedGameObject->GetName().c_str());
            RenderInspectorContent();
        }
        else
        {
            ImGui::Text("No GameObject selected");
        }
        ImGui::End();
    }
    void Inspector::Shutdown()
    {
    }
    void Inspector::SetSelectedGameObject(framework::GameObject *gameObject)
    {
        m_selectedGameObject = gameObject;
    }
    void Inspector::RenderInspectorContent()
    {
        if (!m_selectedGameObject)
            return;

        // 添加组件按钮
        if (ImGui::Button("Add Component", ImVec2(-1, 0)))
        {
            ComponentMenuHelper::OpenAddComponentMenu(m_selectedGameObject);
        }

        // 渲染组件菜单
        ComponentMenuHelper::RenderAddComponentMenu();

        ImGui::Separator();

        // 渲染每个组件检查器，使用新的分组样式
        for (size_t i = 0; i < m_inspectors.size(); ++i)
        {
            auto inspector = m_inspectors[i];
            RenderComponentInspector(inspector, i);
        }
    }

    void Inspector::RenderComponentInspector(ComponentInspector *inspector, size_t index)
    {
        if (!inspector || !m_selectedGameObject)
            return;

        // 检查组件是否存在
        if (!inspector->IsComponentEnabled(m_selectedGameObject) &&
            !HasComponent(inspector, m_selectedGameObject))
            return;

        ImGui::PushID(static_cast<int>(index));

        // === 组件头部区域 ===
        bool componentEnabled = inspector->IsComponentEnabled(m_selectedGameObject);
        bool expanded = RenderComponentHeader(inspector, componentEnabled);

        // 右侧操作按钮
        ImGui::SameLine();
        RenderComponentActions(inspector);

        // === 组件内容区域 ===
        if (expanded && componentEnabled)
        {
            ImGui::Indent();
            inspector->Inspect(m_selectedGameObject);
            ImGui::Unindent();
        }

        ImGui::PopID();
        ImGui::Spacing(); // 组件间距
    }

    bool Inspector::RenderComponentHeader(ComponentInspector *inspector, bool &enabled)
    {
        std::string displayName = inspector->GetComponentDisplayName();

        // 使用 CollapsingHeader 创建可折叠的头部
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen |
                                   ImGuiTreeNodeFlags_AllowItemOverlap;

        // 如果组件被禁用，使用灰色文本
        if (!enabled)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        }

        bool expanded = ImGui::CollapsingHeader(displayName.c_str(), flags);

        if (!enabled)
        {
            ImGui::PopStyleColor();
        }

        // 在同一行添加启用切换框
        ImGui::SameLine();
        bool previousEnabled = enabled;
        if (ImGui::Checkbox("##enabled", &enabled))
        {
            // 启用状态改变时，更新组件
            inspector->SetComponentEnabled(m_selectedGameObject, enabled);
        }

        return expanded;
    }

    void Inspector::RenderComponentActions(ComponentInspector *inspector)
    {
        float buttonWidth = 20.0f;

        // 右对齐操作按钮
        float windowWidth = ImGui::GetWindowWidth();
        float buttonsWidth = buttonWidth * 2 + ImGui::GetStyle().ItemSpacing.x;
        ImGui::SameLine(windowWidth - buttonsWidth - ImGui::GetStyle().WindowPadding.x);

        // 更多操作按钮（暂时禁用）
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        if (ImGui::SmallButton("..."))
        {
            // 预留：显示更多操作菜单
        }
        ImGui::PopStyleVar();
        ImGui::PopItemFlag();

        // 删除按钮
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        if (ImGui::SmallButton("X"))
        {
            // 尝试删除组件
            if (inspector->RemoveComponent(m_selectedGameObject))
            {
                // 删除成功，可以添加日志或反馈
            }
        }
        ImGui::PopStyleColor(2);
    }

    bool Inspector::HasComponent(ComponentInspector *inspector, framework::GameObject *obj)
    {
        // 简单检查：尝试调用 IsComponentEnabled，如果返回 false 且组件不存在则返回 false
        // 这里可能需要根据具体的组件类型进行更精确的检查
        std::string displayName = inspector->GetComponentDisplayName();

        if (displayName == "Transform")
        {
            return obj->HasComponent<framework::Transform>();
        }
        else if (displayName == "Mesh Renderer")
        {
            return obj->HasComponent<framework::MeshRenderer>();
        }
        else if (displayName == "Light")
        {
            return obj->HasComponent<framework::Light>();
        }

        return false;
    }
}
