#pragma once
#include "Framework/Graphic/RenderComponent.h"
#include "Framework/Graphic/Buffer.h"
namespace framework
{

    class MeshFilter;
    class GameObject;
    class Material;
    /**
     * @brief 网格渲染器组件
     * 负责渲染MeshFilter中的网格数据
     */
    class MeshRenderer : public RenderComponent
    {
    public:
        MeshRenderer(GameObject *owner);
        virtual ~MeshRenderer() = default;

        // RenderComponent接口实现
        void Render(Renderer *renderer) override;
        virtual void OnCreate() override;

        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;

        virtual void Deserialize(const rapidjson::Value &jsonValue) override;
        virtual void OnComponentAdd(Component *component) override;

    private:
        MeshFilter *m_meshFilter = nullptr; // 缓存MeshFilter组件引用

        // 获取MeshFilter组件
        MeshFilter *GetMeshFilter();
    };

} // namespace framework
