
#pragma once
#include "Framework/Component/Component.h"
#include "glm/glm.hpp"
namespace framework
{
    enum class ProjectionType
    {
        Perspective, // 透视投影
        Orthographic // 正交投影
    };

    /**
     * @brief 摄像机组件类
     * 用于处理摄像机的视图和投影矩阵
     */
    class Camera : public ComponentBase<Camera>
    {
    public:
        explicit Camera(GameObject *gameObject) : ComponentBase(gameObject) {}

    protected:
        void OnEnable() override;

        void OnDisable() override;

    public:

        // 返回组件名称
        const char *GetName() const override { return "Camera"; }

        // 序列化为JSON值
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;

        // 从JSON值反序列化
        void Deserialize(const rapidjson::Value &jsonValue) override;

        // 获取视图矩阵
        glm::mat4 GetViewMatrix();

        // 获取投影矩阵
        glm::mat4 GetProjectionMatrix();

        // 获取视图投影矩阵
        glm::mat4 GetViewProjectionMatrix();

        // 设置视图矩阵
        void SetViewMatrix(const glm::mat4 &view);

        // 设置投影矩阵
        void SetProjectionMatrix(const glm::mat4 &projection);

        // 设置投影类型
        void SetProjectionType(ProjectionType type);

        void SetMainCamera(bool isMain);
        bool IsMainCamera() const { return isMainCamera; }

        // 设置视场角（仅在透视投影时有效）
        void SetFieldOfView(float fov);

        // 设置宽高比
        void SetAspectRatio(float ratio);

        // 设置近裁剪面
        void SetNearPlane(float near);

        // 设置远裁剪面
        void SetFarPlane(float far);

        void SetPerspectiveProjection(float fov, float aspect, float near, float far);
        void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
        glm::vec4 clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色

    private:
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

    private:
        glm::mat4 viewMatrix;       // 视图矩阵
        glm::mat4 projectionMatrix; // 投影矩阵

        bool isMainCamera = false;     // 是否为主
        bool isProjectionDirty = true; // 投影矩阵是否需要更新
        bool isViewDirty = true;       // 视图矩阵是否需要更新

        ProjectionType projectionType = ProjectionType::Perspective; // 投影类型

        float fieldOfView = 45.0f;                     // 视场角（仅在透视投影时有效）
        float aspectRatio = 16.0f / 9.0f;              //
        float nearPlane = 0.1f;                        // 近裁剪面
        float farPlane = 100.0f;                       // 远裁剪面
        glm::vec2 orthoSize = glm::vec2(10.0f, 10.0f); // 正交投影时的大小
        glm::vec2 orthoOffset = glm::vec2(0.0f, 0.0f); // 正交投影时的偏移
    };
}
