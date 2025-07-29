#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Graphic/MeshRenderer.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Graphic/Material.h"

namespace editor
{
    class MeshRendererInspector : public TypedComponentInspector<framework::MeshRenderer>
    {
    public:
        MeshRendererInspector() : TypedComponentInspector<framework::MeshRenderer>() {}
        ~MeshRendererInspector() override = default;

        void Inspect(framework::GameObject *node) override;

        // 组件显示名称
        std::string GetComponentDisplayName() const override;
        void Update(float deltaTime) override;

    private:
        void RenderMaterial(framework::GameObject *node, framework::MeshRenderer *meshRenderer);
        void RenderMaterialTextures(framework::Material *material);
        class MaterialAssetInspector *materialAssetInspector = nullptr;
    };
}
