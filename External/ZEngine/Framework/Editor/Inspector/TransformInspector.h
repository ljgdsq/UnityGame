#pragma once
#include "Framework/Editor/Inspector/ComponentInspector.h"
namespace editor
{
    class TransformInspector : public TypedComponentInspector<framework::Transform>
    {
    public:
        TransformInspector() = default;
        ~TransformInspector() override = default;

        // Inspect方法用于检查和渲染Transform组件的属性
        void Inspect(framework::GameObject *node) override;

        // 组件显示名称
        std::string GetComponentDisplayName() const override;

    private:
        // 渲染位置、旋转和缩放属性
        void RenderPosition(framework::GameObject *node);
        void RenderRotation(framework::GameObject *node);
        void RenderScale(framework::GameObject *node);
    };
} // namespace editor
