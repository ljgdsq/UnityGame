#include "Framework/Render/MeshFilter.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"

namespace framework
{

    void MeshFilter::SetMesh(const std::string &meshAssetId)
    {
        m_meshAssetRef = AssetReference<MeshAsset>(meshAssetId);
        m_directMesh.reset(); // 清除直接设置的mesh

        Logger::Debug("MeshFilter: Set mesh asset ID: {}", meshAssetId);
    }

    void MeshFilter::SetMesh(std::shared_ptr<MeshAsset> meshAsset)
    {
        if (meshAsset)
        {
            m_meshAssetRef = AssetReference<MeshAsset>(meshAsset->GetAssetId());
            m_directMesh.reset(); // 清除直接设置的mesh

            Logger::Debug("MeshFilter: Set mesh asset: {}", meshAsset->GetName());
        }
        else
        {
            m_meshAssetRef = AssetReference<MeshAsset>();
            m_directMesh.reset();

            Logger::Debug("MeshFilter: Cleared mesh asset");
        }
    }

    void MeshFilter::SetMeshAssetReference(const AssetReference<MeshAsset> &meshAssetRef)
    {
        m_meshAssetRef = meshAssetRef;
        m_directMesh.reset(); // 清除直接设置的mesh

        Logger::Debug("MeshFilter: Set mesh asset reference: {}", meshAssetRef.GetAssetId());
    }

    std::shared_ptr<MeshAsset> MeshFilter::GetMeshAsset() const
    {
        return m_meshAssetRef.Get();
    }

    std::shared_ptr<Mesh> MeshFilter::GetMesh() const
    {
        // 优先返回直接设置的mesh（兼容性）
        if (m_directMesh)
        {
            return m_directMesh;
        }

        // 从AssetReference获取mesh
        auto meshAsset = m_meshAssetRef.Get();
        if (meshAsset && meshAsset->IsLoaded())
        {
            return meshAsset->GetMesh();
        }

        return nullptr;
    }

    bool MeshFilter::HasMesh() const
    {
        // 检查直接设置的mesh
        if (m_directMesh && !m_directMesh->GetVertices().empty())
        {
            return true;
        }

        // 检查AssetReference中的mesh
        auto meshAsset = m_meshAssetRef.Get();
        if (meshAsset && meshAsset->IsLoaded())
        {
            auto mesh = meshAsset->GetMesh();
            return mesh && !mesh->GetVertices().empty();
        }

        return false;
    }

    void MeshFilter::SetMesh(std::shared_ptr<Mesh> mesh)
    {
        // 兼容性方法：直接设置mesh对象
        m_directMesh = mesh;
        m_meshAssetRef = AssetReference<MeshAsset>(); // 清除asset引用

        if (mesh)
        {
            Logger::Debug("MeshFilter: Set mesh directly (compatibility mode)");
        }
        else
        {
            Logger::Debug("MeshFilter: Cleared mesh");
        }
    }

    void MeshFilter::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (jsonValue.HasMember("meshAssetId") && jsonValue["meshAssetId"].IsString())
        {
            std::string meshAssetId = jsonValue["meshAssetId"].GetString();
            if (!meshAssetId.empty())
            {
                SetMesh(meshAssetId);
            }
        }
        else if (jsonValue.HasMember("meshAssetRef") && jsonValue["meshAssetRef"].IsObject())
        {
            // 反序列化AssetReference
            AssetReference<MeshAsset> meshAssetRef;
            meshAssetRef.Deserialize(jsonValue["meshAssetRef"]);
            SetMeshAssetReference(meshAssetRef);
        }

        // 兼容性：支持旧的序列化格式
        // if (jsonValue.HasMember("mesh")) {
        //     // 这里可以支持直接序列化mesh数据的旧格式
        // }
    }

    rapidjson::Value MeshFilter::Serialize() const
    {
        rapidjson::Document doc;
        doc.SetObject();
        auto &allocator = doc.GetAllocator();

        rapidjson::Value jsonValue(rapidjson::kObjectType);

        // 序列化AssetReference
        if (m_meshAssetRef.IsValid())
        {
            rapidjson::Value meshAssetRefValue = m_meshAssetRef.Serialize(allocator);
            jsonValue.AddMember("meshAssetRef", meshAssetRefValue, allocator);

            // 同时保存assetId用于简单读取
            rapidjson::Value assetIdValue(m_meshAssetRef.GetAssetId().c_str(), allocator);
            jsonValue.AddMember("meshAssetId", assetIdValue, allocator);
        }

        // 注意：我们不序列化直接设置的mesh，因为它们应该是临时的

        return jsonValue;
    }

} // namespace framework
