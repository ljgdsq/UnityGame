#include "MeshRenderer.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Render/Renderer.h"
#include "Framework/Render/Material.h"
#include "Framework/Component/Transform.h"

namespace framework {

MeshRenderer::MeshRenderer(GameObject* owner) 
    : ComponentBase<MeshRenderer>(owner) {
}


void MeshRenderer::Render(Renderer* renderer) {
    if (!m_visible || !m_meshFilter) {
        return;
    }
    
    auto mesh = m_meshFilter->GetMesh();
    if (!mesh || mesh->GetVertices().empty()) {
        return;
    }
    
    // 获取Transform组件
    Transform* transform = GetOwner()->GetComponent<Transform>();
    
    // 设置变换矩阵
    glm::mat4 modelMatrix = transform->GetWorldMatrix();
    
    // 使用材质渲染网格
    if (m_material) {
        m_material->Use();
        m_material->SetMatrix("model", modelMatrix);
    }
    
   glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, mesh->GetIndices().data());
}

void MeshRenderer::OnInitialize()
{
    m_meshFilter = GetMeshFilter();
}

MeshFilter* MeshRenderer::GetMeshFilter() {
    if (!m_meshFilter) {
        m_meshFilter = GetOwner()->GetComponent<MeshFilter>();
    }
    return m_meshFilter;
}

} // namespace framework
