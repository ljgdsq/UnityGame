#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Core/GameObject.h"

namespace editor
{
    class MeshFilterInspector : public TypedComponentInspector<framework::MeshFilter>
    {
    public:
        void Inspect(framework::GameObject *node) override;

        // 组件显示名称
        std::string GetComponentDisplayName() const override;

    private:
        void RenderMeshAssetField(framework::MeshFilter *meshFilter);
        void RenderMeshInfo(framework::MeshFilter *meshFilter);
    };
}
