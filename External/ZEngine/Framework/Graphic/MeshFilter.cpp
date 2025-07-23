#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"

namespace framework
{


    void MeshFilter::SetMesh(std::shared_ptr<MeshAsset> meshAsset)
    {
        if (meshAsset)
        {

            m_meshAsset = meshAsset;
            Logger::Debug("MeshFilter: Set mesh asset: {}", meshAsset->GetName());
        }
        else
        {

            m_meshAsset.reset();
            Logger::Debug("MeshFilter: Cleared mesh asset");
        }
    }



    std::shared_ptr<MeshAsset> MeshFilter::GetMeshAsset() const
    {
        return m_meshAsset;
    }

    std::shared_ptr<Mesh> MeshFilter::GetMesh() const
    {
        // 优先返回直接设置的mesh（兼容性）
        if (m_meshAsset)
        {
            return m_meshAsset->GetMesh();
        }

        return nullptr;
    }

    bool MeshFilter::HasMesh() const
    {
        // 检查直接设置的mesh
        if (m_meshAsset && m_meshAsset->GetMesh())
        {
            return true;
        }


        return false;
    }

    void MeshFilter::Deserialize(const rapidjson::Value &jsonValue)
    {

        // 兼容性：支持旧的序列化格式
        // if (jsonValue.HasMember("mesh")) {
        //     // 这里可以支持直接序列化mesh数据的旧格式
        // }
    }

    rapidjson::Value MeshFilter::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {

        rapidjson::Value jsonValue(rapidjson::kObjectType);

        return jsonValue;
    }

} // namespace framework
