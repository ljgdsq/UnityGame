#pragma once
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace editor
{
    class TransformInspector;
}

namespace framework
{
    class GameObject;
    class Transform : public ComponentBase<Transform>
    {
        friend class editor::TransformInspector;

    public:
        explicit Transform(GameObject *gameObject) : ComponentBase(gameObject) {}

        // Serializes the component to a JSON value
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &jsonValue) override;

        // Getters and setters for position, rotation, scale
        glm::vec3 GetPosition() const { return position; }
        void SetPosition(const glm::vec3 &pos) { position = pos; }

        glm::quat GetRotation() const { return rotation; }
        void SetRotation(const glm::quat &rot) { rotation = rot; }

        // 设置欧拉角旋转
        void SetRotation(const glm::vec3 &eulerAngles);

        // 旋转变换
        void Rotate(const glm::vec3 &axis, float angle);

        glm::vec3 GetScale() const { return scale; }
        void SetScale(const glm::vec3 &scl) { scale = scl; }

        // 获取模型矩阵
        glm::mat4 GetModelMatrix() const;

        // 获取方向向量
        glm::vec3 GetForward() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

    private:
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
    };
} // namespace framework
