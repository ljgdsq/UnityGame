#include "Framework/Editor/Inspector.h"
#include "Framework/Editor/Inspector/TransformInspector.h"
#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Editor/EditorContext.h"
#include "Framework/Editor/Inspector/LightInspector.h"
#include "Framework/Editor/ComponentMenuHelper.h"
namespace editor
{

    Inspector::Inspector() : EditorWidget("Inspector"),
                             m_selectedGameObject(nullptr)
    {
        // 初始化组件检查器
        m_inspectors.push_back(new TransformInspector());
        // 可以在这里添加更多的组件检查器
        m_inspectors.push_back(new MeshRendererInspector());

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

        // 遍历所有组件检查器并调用Inspect方法
        for (auto inspector : m_inspectors)
        {
            inspector->Inspect(m_selectedGameObject);
        }
    }
}
