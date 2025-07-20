#pragma once
#include "Framework/Component/Component.h"
#include "Framework/Graphic/Mesh.h"
#include "Framework/Asset/AssetReference.h"
#include "Framework/Asset/MeshAsset.h"
#include <memory>

namespace framework
{

    /**
     * @brief 网格过滤器组件
     * 存储和管理网格数据，现在使用AssetReference进行资源管理
     */
    class MeshFilter : public ComponentBase<MeshFilter>
    {
    public:
        MeshFilter(GameObject *owner) : ComponentBase<MeshFilter>(owner) {
                                            // 在构造函数中可以进行初始化操作
                                        };
        ~MeshFilter() = default;

        // 使用AssetReference设置网格
        void SetMesh(const std::string &meshAssetId);
        void SetMesh(std::shared_ptr<MeshAsset> meshAsset);
        void SetMeshAssetReference(const AssetReference<MeshAsset> &meshAssetRef);

        // 获取资源引用
        const AssetReference<MeshAsset> &GetMeshAssetReference() const { return m_meshAssetRef; }
        AssetReference<MeshAsset> &GetMeshAssetReference() { return m_meshAssetRef; }

        // 获取资源对象
        std::shared_ptr<MeshAsset> GetMeshAsset() const;
        std::shared_ptr<Mesh> GetMesh() const; // 便利方法，直接获取Mesh对象

        // 检查是否有有效网格
        bool HasMesh() const;

        // 兼容性方法（保留原有接口）
        void SetMesh(std::shared_ptr<Mesh> mesh);

        // 序列化支持
        virtual void Deserialize(const rapidjson::Value &jsonValue) override;
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;

    private:
        AssetReference<MeshAsset> m_meshAssetRef; // 网格资源引用

        // 临时存储直接设置的Mesh对象（用于兼容性）
        std::shared_ptr<Mesh> m_directMesh;
    };

} // namespace framework
