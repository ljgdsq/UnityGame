#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Component/Camera.h"
namespace editor
{
    class CameraInspector : public TypedComponentInspector<framework::Camera>
    {
    public:
        CameraInspector() = default;
        ~CameraInspector() override = default;

        void Update(float deltaTime) override;

        void Inspect(framework::GameObject *node) override;

        std::string GetComponentDisplayName() const override;
    };

} // namespace editor

