#pragma once
#include "Framework/Component/Component.h"
#include "Framework/Graphic/Mesh.h"
#include "Framework/Asset/MeshAsset.h"
#include <memory>

namespace framework
{

    /**
     * @brief 网格过滤器组件
     * 存储和管理网格数据
     */
    class MeshFilter : public ComponentBase<MeshFilter>
    {
    public:
        MeshFilter(GameObject *owner) : ComponentBase<MeshFilter>(owner) {
                                            // 在构造函数中可以进行初始化操作
                                        };
        ~MeshFilter() = default;

        // 使用AssetReference设置网格

        void SetMesh(std::shared_ptr<MeshAsset> meshAsset);

        // 获取资源对象
        std::shared_ptr<MeshAsset> GetMeshAsset() const;
        std::shared_ptr<Mesh> GetMesh() const; // 便利方法，直接获取Mesh对象

        // 检查是否有有效网格
        bool HasMesh() const;

        // 序列化支持
        virtual void Deserialize(const rapidjson::Value &jsonValue) override;
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;

    private:

        // 临时存储直接设置的Mesh对象（用于兼容性）
        std::shared_ptr<MeshAsset> m_meshAsset;
    };

} // namespace framework
