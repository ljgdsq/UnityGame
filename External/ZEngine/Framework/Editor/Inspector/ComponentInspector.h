#pragma once
#include "imgui.h"
#include <string>
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
namespace editor
{
    class ComponentInspector
    {
    public:
        ComponentInspector() = default;
        virtual ~ComponentInspector() = default;
        virtual void Update(float deltaTime) {}

    public:
        virtual void Inspect(framework::GameObject *node) = 0;
    };
}