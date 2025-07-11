#include "Framework/Editor/Inspector.h"
#include "Framework/Editor/Inspector/TransformInspector.h"
#include "Framework/Editor/Inspector/MeshRendererInspector.h"
#include "Framework/Editor/EditorContext.h"
namespace editor
{

    Inspector::Inspector(): EditorWidget("Inspector"),
      m_selectedGameObject(nullptr)
    {
        // 初始化组件检查器
        m_inspectors.push_back(new TransformInspector());
        // 可以在这里添加更多的组件检查器
        m_inspectors.push_back(new MeshRendererInspector());
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

        // 遍历所有组件检查器并调用Inspect方法
        for (auto inspector : m_inspectors)
        {
            inspector->Inspect(m_selectedGameObject);
        }
    }
}
