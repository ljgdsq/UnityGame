#include "Framework/Editor/ThumbnailGenerator.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Graphic/Buffer.h"
#include "glm/glm.hpp"
#include "Framework/Asset/ShaderManager.h"
#include "Framework/Asset/TextureAsset.h"
namespace editor
{
    using namespace framework;

    std::shared_ptr<TextureAsset> ThumbnailGenerator::GenerateMeshThumbnail(std::shared_ptr<MeshAsset> mesh, const ThumbnailConfig &config)
    {
        if (!mesh)
        {
            Logger::Error("ThumbnailGenerator: Cannot generate thumbnail for null mesh");
            return nullptr;
        }

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        SetupFrameBuffer(config, mesh->GetName());
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
        SetupMatrices(config);
        mesh->GetMesh()->Use();
        glDrawElements(GL_TRIANGLES, mesh->GetMesh()->GetIndices().size(), GL_UNSIGNED_INT, 0);

        m_frameBuffers[mesh->GetName()]->UnbindBuffer();

        // 恢复viewport
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        auto colorBuffer = m_frameBuffers[mesh->GetName()]->GetColorBuffer();

        auto asset = std::make_shared<TextureAsset>(mesh->GetName());
        auto tex = asset->GetTexture();

        return std::make_shared<TextureAsset>(colorBuffer, mesh->GetName(), config.width, config.height, 4);
    }

    void ThumbnailGenerator::SetupFrameBuffer(const ThumbnailConfig &config, const std::string &name)
    {

        m_frameBuffers[name] = std::make_unique<FrameBuffer>(config.width, config.height);

        m_frameBuffers[name]->BindBuffer();
        glViewport(0, 0, config.width, config.height);

        // 清除缓冲区
        glClearColor(config.backgroundColor.r, config.backgroundColor.g,
                     config.backgroundColor.b, config.backgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ThumbnailGenerator::SetupMatrices(const ThumbnailConfig &config)
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

    std::shared_ptr<framework::TextureAsset>
    ThumbnailGenerator::GenerateThumbnail(std::shared_ptr<framework::Asset> asset, const ThumbnailConfig &config)
    {
        if (!asset)
        {
            Logger::Error("ThumbnailGenerator: Cannot generate thumbnail for null asset");
            return nullptr;
        }

        switch (asset->GetType())
        {
        case AssetType::Mesh:
            return GenerateMeshThumbnail(std::dynamic_pointer_cast<MeshAsset>(asset), config);
            // 其他类型的缩略图生成可以在这里添加
        default:
            Logger::Warn("ThumbnailGenerator: Unsupported asset type for thumbnail generation");
            return nullptr;
        }
        return nullptr;
    }
}
