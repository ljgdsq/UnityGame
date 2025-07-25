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
        if (jsonValue.IsObject())
        {
            if (jsonValue.HasMember("mesh") && jsonValue["mesh"].IsString())
            {
                std::string meshName = jsonValue["mesh"].GetString();
                m_meshAsset = AssetManager::GetInstance().GetAsset<MeshAsset>(meshName);
                if (!m_meshAsset)
                {
                    Logger::Error("MeshFilter: Failed to load mesh asset: {}", meshName);
                }
            }
        }
    }

    rapidjson::Value MeshFilter::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {

        rapidjson::Value jsonValue(rapidjson::kObjectType);
        if (m_meshAsset)
        {
            jsonValue.AddMember("mesh", rapidjson::Value(m_meshAsset->GetName().c_str(), allocator), allocator);
        }
        jsonValue.AddMember("type", rapidjson::Value("MeshFilter", allocator), allocator);

        return jsonValue;
    }

} // namespace framework
