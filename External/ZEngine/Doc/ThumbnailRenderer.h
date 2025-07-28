#pragma once
#include <memory>
#include "glm/glm.hpp"

namespace framework
{
    class Mesh;
    class Shader;
    class FrameBuffer;

    struct ThumbnailConfig
    {
        int width = 128;
        int height = 128;
        glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        glm::vec3 rotationX = glm::vec3(45.0f, 0.0f, 0.0f); // 绕X轴旋转角度
        glm::vec3 rotationY = glm::vec3(0.0f, 30.0f, 0.0f); // 绕Y轴旋转角度
        glm::vec4 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    };

    class ThumbnailRenderer
    {
    public:
        static ThumbnailRenderer &GetInstance();

        // 渲染网格缩略图
        void *RenderMeshThumbnail(std::shared_ptr<Mesh> mesh, const ThumbnailConfig &config = ThumbnailConfig{});

        // 渲染材质缩略图
        void *RenderMaterialThumbnail(/* Material参数 */, const ThumbnailConfig &config = ThumbnailConfig{});

        // 清理缩略图资源
        void ReleaseThumbnail(void *thumbnailId);

    private:
        ThumbnailRenderer() = default;
        ~ThumbnailRenderer() = default;

        void SetupMatrices(const ThumbnailConfig &config);
        void SetupFrameBuffer(const ThumbnailConfig &config);
        void *FinalizeThumbnail();

        std::unique_ptr<FrameBuffer> m_frameBuffer;
        std::shared_ptr<Shader> m_thumbnailShader = nullptr;
    };

} // namespace framework
