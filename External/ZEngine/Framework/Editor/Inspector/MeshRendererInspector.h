#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Render/MeshRenderer.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Render/Material.h"

namespace editor
{
    class MeshRendererInspector : public TypedComponentInspector<framework::MeshRenderer>
    {
    public:
        void Inspect(framework::GameObject *node) override;

        // 组件显示名称
        std::string GetComponentDisplayName() const override;

    private:
        void RenderMaterial(framework::GameObject *node);
        void RenderMaterialTextures(framework::Material *material);
    };
}
