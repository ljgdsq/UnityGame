#pragma once
#include "Framework/Component/Component.h"
#include <memory>
#include "glm/glm.hpp"

namespace framework
{
    class MaterialAsset;
    class Renderer;
    class GameObject;
    /**
     * @brief 可渲染组件基类
     * 所有可渲染的组件都应该继承此类
     */
    class RenderComponent : public ComponentBase<RenderComponent>
    {
    public:
        RenderComponent(GameObject *owner)
            : ComponentBase<RenderComponent>(owner) {}
        virtual ~RenderComponent() = default;

        // 渲染接口
        virtual void Render(Renderer *renderer) = 0;

        // 可见性控制
        bool IsVisible() const { return m_visible; }
        void SetVisible(bool visible) { m_visible = visible; }

        // 材质管理
        void SetMaterial(std::shared_ptr<MaterialAsset> material) { m_material = material; }
        std::shared_ptr<MaterialAsset> GetMaterial() const { return m_material; }

        std::shared_ptr<MaterialAsset> GetMaterialSharedPtr() const
        {
            return m_material;
        } // 渲染层级
        int GetRenderLayer() const { return m_renderLayer; }
        void SetRenderLayer(int layer) { m_renderLayer = layer; }

        // 排序顺序
        int GetSortingOrder() const { return m_sortingOrder; }
        void SetSortingOrder(int order) { m_sortingOrder = order; }

        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;
        virtual void Deserialize(const rapidjson::Value &jsonValue);

    protected:
        bool m_visible = true;
        std::shared_ptr<MaterialAsset> m_material; // 材质
        int m_renderLayer = 0;
        int m_sortingOrder = 0;
    };

} // namespace framework
