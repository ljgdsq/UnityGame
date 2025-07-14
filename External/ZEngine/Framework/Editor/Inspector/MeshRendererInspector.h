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

        // 新增：组件信息相关方法
        std::string GetComponentDisplayName() const override;
        bool IsComponentEnabled(framework::GameObject *obj) const override;
        void SetComponentEnabled(framework::GameObject *obj, bool enabled) override;
        bool RemoveComponent(framework::GameObject *obj) override;

    private:
        void RenderMaterial(framework::GameObject *node);
        void RenderMaterialTextures(framework::Material *material);
    };
}
