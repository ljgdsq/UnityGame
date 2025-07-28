#include "Framework/Graphic/ThumbnailRenderer.h"
#include "Framework/Core/Mesh.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Asset/ShaderManager.h"
#include "Framework/Log/Logger.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>

namespace framework
{

    
    ThumbnailRenderer &ThumbnailRenderer::GetInstance()
    {
        static ThumbnailRenderer instance;
        return instance;
    }

    void *ThumbnailRenderer::RenderMeshThumbnail(std::shared_ptr<Mesh> mesh, const ThumbnailConfig &config)
    {
        if (!mesh)
        {
            Logger::Error("ThumbnailRenderer: Cannot render null mesh");
            return nullptr;
        }

        // 设置Frame Buffer
        SetupFrameBuffer(config);

        // 获取着色器
        if (!m_thumbnailShader)
        {
            m_thumbnailShader = ShaderManager::GetInstance().GetShader("Shaders/Thumbnail")->GetShader();
        }

        if (!m_thumbnailShader)
        {
            Logger::Error("ThumbnailRenderer: Failed to load thumbnail shader");
            return nullptr;
        }

        m_thumbnailShader->Use();

        // 设置变换矩阵
        SetupMatrices(config);

        // 绘制网格
        mesh->Use();
        glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);

        return FinalizeThumbnail();
    }

    void ThumbnailRenderer::SetupFrameBuffer(const ThumbnailConfig &config)
    {
        // 保存当前viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // 创建或重用Frame Buffer
        if (!m_frameBuffer ||
            m_frameBuffer->GetWidth() != config.width ||
            m_frameBuffer->GetHeight() != config.height)
        {
            m_frameBuffer = std::make_unique<FrameBuffer>(config.width, config.height);
        }

        m_frameBuffer->BindBuffer();
        glViewport(0, 0, config.width, config.height);

        // 清除缓冲区
        glClearColor(config.backgroundColor.r, config.backgroundColor.g,
                     config.backgroundColor.b, config.backgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ThumbnailRenderer::SetupMatrices(const ThumbnailConfig &config)
    {
        // 创建模型矩阵（应用旋转）
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(config.rotationX.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(config.rotationY.y), glm::vec3(0.0f, 1.0f, 0.0f));

        // 创建视图矩阵
        glm::mat4 view = glm::lookAt(config.cameraPosition, config.targetPosition, config.upDirection);

        // 创建投影矩阵
        float aspectRatio = static_cast<float>(config.width) / static_cast<float>(config.height);
        glm::mat4 projection = glm::perspective(glm::radians(config.fov), aspectRatio, config.nearPlane, config.farPlane);

        // 传递给着色器
        m_thumbnailShader->SetMatrix4("model", model);
        m_thumbnailShader->SetMatrix4("view", view);
        m_thumbnailShader->SetMatrix4("projection", projection);
    }

    void *ThumbnailRenderer::FinalizeThumbnail()
    {
        if (!m_frameBuffer)
        {
            return nullptr;
        }

        m_frameBuffer->UnbindBuffer();

        // 恢复viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        return reinterpret_cast<void *>(m_frameBuffer->GetColorBuffer());
    }

    void ThumbnailRenderer::ReleaseThumbnail(void *thumbnailId)
    {
        // 这里可以实现缩略图资源的清理逻辑
        // 具体实现取决于你的资源管理策略
    }

} // namespace framework
