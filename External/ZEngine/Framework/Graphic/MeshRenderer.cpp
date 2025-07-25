#include "Framework/Core/GameObject.h"
#include "Framework/Graphic/MeshRenderer.h"
#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Graphic/Renderer.h"
#include "Framework/Graphic/Material.h"
#include "Framework/Component/Transform.h"
#include "Framework/Manager/LightManager.h"
#include "Framework/Log/Logger.h"
#include "glad/glad.h"
#include "Framework/Manager/CameraManager.h"
#include "Framework/Asset/AssetManager.h"
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
        auto camera = CameraManager::GetInstance().GetMainCamera();
        if (camera == nullptr)
        {
            return;
        }
        // 确保OpenGL状态正确
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);

        // 使用材质渲染网格
        if (m_material)
        {
            // 应用光照到材质
            LightManager::ApplyLights(m_material);

            // 输出完整矩阵用于调试
            glm::mat4 viewMatrix = camera->GetViewMatrix();
            glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
            m_material->Use();
            m_material->SetMatrix("model", modelMatrix);
            m_material->SetMatrix("view", viewMatrix);
            m_material->SetMatrix("projection", projectionMatrix);
        }
        mesh->Use();

        // 绘制元素（使用 EBO 时最后一个参数应该是 0）
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndices().size()), GL_UNSIGNED_INT, 0);
    }

    void MeshRenderer::OnCreate()
    {
        Component::OnCreate();
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

    rapidjson::Value MeshRenderer::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);
        // jsonValue.AddMember("type", "MeshRenderer", allocator);
        // if (m_meshFilter && m_meshFilter->HasMesh())
        // {
        //     jsonValue.AddMember("mesh", m_meshFilter->GetMeshAsset()->GetName(), allocator);
        // }
        return jsonValue;
    }

    void MeshRenderer::Deserialize(const rapidjson::Value &jsonValue)
    {

    }

} // namespace framework
