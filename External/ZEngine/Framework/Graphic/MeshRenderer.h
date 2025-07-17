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
        virtual void OnInitialize() override;

        virtual rapidjson::Value Serialize() const override
        {
            // 序列化为JSON值
            // rapidjson::Value jsonValue(rapidjson::kObjectType);
            // if (m_material) {
            //     jsonValue.AddMember("material", m_material->Serialize(), GetAllocator());
            // }
            // return jsonValue;
            return rapidjson::Value(); // 返回空值，实际实现中应返回有效的JSON值
        }

        virtual void Deserialize(const rapidjson::Value &jsonValue) override
        {
            // 从JSON值反序列化材质
            // if (jsonValue.HasMember("material")) {
            //     m_material = Material::Deserialize(jsonValue["material"]);
            // }
        }

    private:
        MeshFilter *m_meshFilter = nullptr; // 缓存MeshFilter组件引用

        // 获取MeshFilter组件
        MeshFilter *GetMeshFilter();
    };

} // namespace framework
