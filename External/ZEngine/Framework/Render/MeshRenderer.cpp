#include "Framework/Core/GameObject.h"
#include "Framework/Render/MeshRenderer.h"
#include "Framework/Render/MeshFilter.h"
#include "Framework/Render/Renderer.h"
#include "Framework/Render/Material.h"
#include "Framework/Component/Transform.h"
#include "glad/glad.h"
namespace framework
{

    MeshRenderer::MeshRenderer(GameObject *owner)
        : RenderComponent(owner)
    {
    }

    void MeshRenderer::Render(Renderer *renderer)
    {
        if (!m_visible || !m_meshFilter)
        {
            return;
        }

        auto mesh = m_meshFilter->GetMesh();
        if (!mesh || mesh->GetVertices().empty())
        {
            return;
        }

        // 获取Transform组件
        Transform *transform = GetGameObject()->GetComponent<Transform>();

        // 设置变换矩阵
        glm::mat4 modelMatrix = transform->GetModelMatrix();

        // 使用材质渲染网格
        if (m_material)
        {
            m_material->Use();
            m_material->SetMatrix("model", modelMatrix);
        }

        glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, mesh->GetIndices().data());
    }

    void MeshRenderer::OnInitialize()
    {
        m_meshFilter = GetMeshFilter();
    }

    MeshFilter *MeshRenderer::GetMeshFilter()
    {
        if (!m_meshFilter)
        {
            m_meshFilter = GetGameObject()->GetComponent<MeshFilter>();
        }
        return m_meshFilter;
    }

} // namespace framework
