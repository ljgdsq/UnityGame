#pragma once
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
namespace framework
{
    class GameObject;
    class Transform : public ComponentBase<Transform>
    {
    public:
        explicit Transform(GameObject *gameObject) : ComponentBase(gameObject) {}

        // Returns the name of the component
        const char *GetName() const override { return "Transform"; }

        // Serializes the component to a JSON value
        rapidjson::Value Serialize() const override;
        void Deserialize(const rapidjson::Value& jsonValue) override;

        // Getters and setters for position, rotation, scale
        glm::vec3 GetPosition() const { return position; }
        void SetPosition(const glm::vec3 &pos) { position = pos; }

        glm::quat GetRotation() const { return rotation; }
        void SetRotation(const glm::quat &rot) { rotation = rot; }

        glm::vec3 GetScale() const { return scale; }
        void SetScale(const glm::vec3 &scl) { scale = scl; }

    private:
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
    };
} // namespace framework