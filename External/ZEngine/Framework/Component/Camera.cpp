// Camera.cpp
#include "Framework/Component/Camera.h"
#include "Framework/Component/Transform.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Log/Logger.h"
#include "Framework/Manager/CameraManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
namespace framework
{

    rapidjson::Value Camera::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        // 序列化组件类型
        obj.AddMember("type", "Camera", allocator);
        // 序列化基本属性
        obj.AddMember("isMainCamera", isMainCamera, allocator);
        obj.AddMember("projectionType", static_cast<int>(projectionType), allocator);
        obj.AddMember("fieldOfView", fieldOfView, allocator);
        obj.AddMember("aspectRatio", aspectRatio, allocator);
        obj.AddMember("nearPlane", nearPlane, allocator);
        obj.AddMember("farPlane", farPlane, allocator);

        // 序列化清除颜色
        rapidjson::Value clearColorArr(rapidjson::kArrayType);
        clearColorArr.PushBack(clearColor.r, allocator);
        clearColorArr.PushBack(clearColor.g, allocator);
        clearColorArr.PushBack(clearColor.b, allocator);
        clearColorArr.PushBack(clearColor.a, allocator);
        obj.AddMember("clearColor", clearColorArr, allocator);

        return obj;
    }

    void Camera::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (jsonValue.HasMember("isMainCamera") && jsonValue["isMainCamera"].IsBool())
        {
            isMainCamera = jsonValue["isMainCamera"].GetBool();
        }

        if (jsonValue.HasMember("projectionType") && jsonValue["projectionType"].IsInt())
        {
            projectionType = static_cast<ProjectionType>(jsonValue["projectionType"].GetInt());
        }

        if (jsonValue.HasMember("fieldOfView") && jsonValue["fieldOfView"].IsDouble())
        {
            fieldOfView = jsonValue["fieldOfView"].GetDouble();
        }

        if (jsonValue.HasMember("aspectRatio") && jsonValue["aspectRatio"].IsDouble())
        {
            aspectRatio = jsonValue["aspectRatio"].GetDouble();
        }

        if (jsonValue.HasMember("nearPlane") && jsonValue["nearPlane"].IsDouble())
        {
            nearPlane = jsonValue["nearPlane"].GetDouble();
        }

        if (jsonValue.HasMember("farPlane") && jsonValue["farPlane"].IsDouble())
        {
            farPlane = jsonValue["farPlane"].GetDouble();
        }

        if (jsonValue.HasMember("clearColor") && jsonValue["clearColor"].IsArray() &&
            jsonValue["clearColor"].Size() == 4)
        {
            const auto &arr = jsonValue["clearColor"];
            clearColor = glm::vec4(
                arr[0].GetDouble(),
                arr[1].GetDouble(),
                arr[2].GetDouble(),
                arr[3].GetDouble());
        }
        isProjectionDirty = true;
        isViewDirty = true;
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        if (isViewDirty)
        {
            RecalculateViewMatrix();
        }

        return viewMatrix;
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        if (isProjectionDirty)
        {
            RecalculateProjectionMatrix();
        }

        return projectionMatrix;
    }

    glm::mat4 Camera::GetViewProjectionMatrix()
    {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    void Camera::SetViewMatrix(const glm::mat4 &view)
    {
        viewMatrix = view;
    }

    void Camera::SetProjectionMatrix(const glm::mat4 &projection)
    {
        projectionMatrix = projection;
    }

    void Camera::SetProjectionType(ProjectionType type)
    {
        if (projectionType != type)
        {
            projectionType = type;
            isProjectionDirty = true;
        }
    }

    void Camera::SetMainCamera(bool isMain)
    {
        isMainCamera = isMain;
    }
    void Camera::SetFieldOfView(float fov)
    {
        if (fieldOfView != fov)
        {
            fieldOfView = fov;
            if (projectionType == ProjectionType::Perspective)
                isProjectionDirty = true;
            else
                Logger::Warn("Field of view is only applicable for perspective projection.");
        }
    }
    void Camera::SetAspectRatio(float ratio)
    {
        if (aspectRatio != ratio)
        {
            aspectRatio = ratio;
            isProjectionDirty = true;
        }
    }
    void Camera::SetNearPlane(float near)
    {
        if (nearPlane != near)
        {
            nearPlane = near;
            isProjectionDirty = true;
        }
    }
    void Camera::SetFarPlane(float far)
    {
        if (farPlane != far)
        {
            farPlane = far;
            isProjectionDirty = true;
        }
    }

    void Camera::SetPerspectiveProjection(float fov, float aspect, float near, float far)
    {
        projectionType = ProjectionType::Perspective;
        fieldOfView = fov;
        aspectRatio = aspect;
        nearPlane = near;
        farPlane = far;
        isProjectionDirty = true;
    }

    void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
    {
        projectionType = ProjectionType::Orthographic;

        // 这里可以设置正交投影的参数
        orthoSize = glm::vec2(right - left, top - bottom);
        orthoOffset = glm::vec2((right + left) / 2.0f, (top + bottom) / 2.0f);
        aspectRatio = (right - left) / (top - bottom);
        nearPlane = near;
        farPlane = far;
        isProjectionDirty = true;
    }
    void Camera::RecalculateViewMatrix()
    {
        Transform *transform = GetGameObject()->GetComponent<Transform>();
        if (!transform)
        {
            Logger::Error("Camera requires a Transform component to calculate the view matrix.");
            return;
        }

        glm::vec3 position = transform->GetPosition();
        glm::vec3 forward;
        glm::vec3 up;

        // 四元数
        glm::quat rotation = transform->GetRotation();
        forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

        viewMatrix = glm::lookAt(position, position + forward, up);
        isViewDirty = false;

        oldPos = position;
        oldRotate = rotation;
    }
    void Camera::RecalculateProjectionMatrix()
    {
        if (projectionType == ProjectionType::Perspective)
        {
            projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
        }
        else if (projectionType == ProjectionType::Orthographic)
        {
            float left = orthoOffset.x - orthoSize.x / 2.0f;
            float right = orthoOffset.x + orthoSize.x / 2.0f;
            float bottom = orthoOffset.y - orthoSize.y / 2.0f;
            float top = orthoOffset.y + orthoSize.y / 2.0f;

            projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
        }
        isProjectionDirty = false;
    }

    void Camera::OnEnable()
    {
        Component::OnEnable();
        CameraManager::GetInstance().RegisterCamera(this);
    }

    void Camera::OnDisable()
    {
        Component::OnDisable();
        CameraManager::GetInstance().UnregisterCamera(this);
    }

    void Camera::OnUpdate(float deltaTime)
    {
        Component::OnUpdate(deltaTime);
        if (isViewDirty || isProjectionDirty)
        {
            return;
        }

        Transform *transform = gameObject->GetComponent<Transform>();
        glm::vec3 position = transform->GetPosition();
        glm::quat rotation = transform->GetRotation();
        // 检查位置和旋转是否发生变化 float 可能有误差避免
        if (glm::length(position - oldPos) > 0.001f)
        {
            isViewDirty = true;
            return;
        }

        if (abs(rotation.x - oldRotate.x) > 0.001f ||
            abs(rotation.y - oldRotate.y) > 0.001f ||
            abs(rotation.z - oldRotate.z) > 0.001f ||
            abs(rotation.w - oldRotate.w) > 0.001f)
        {
            isViewDirty = true;
            return;
        }
    }
} // namespace framework
