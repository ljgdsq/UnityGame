#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Render/MeshRenderer.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Render/Material.h"
namespace editor
{
    class MeshRendererInspector : public ComponentInspector
    {
    public:
        void Inspect(framework::GameObject *node) override;

    private:
        void RenderMaterial(framework::GameObject *node);
        void RenderMaterialTextures(framework::Material* material);
    };
}