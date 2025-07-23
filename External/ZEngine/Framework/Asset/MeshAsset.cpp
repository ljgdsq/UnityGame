#include "Framework/Asset/MeshAsset.h"
#include "Framework/Core/EngineFileIO.h"
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace framework
{
    MeshAsset::MeshAsset(const std::string &name)
        : Asset(name, AssetType::Mesh)
    {
        Logger::Debug("Created MeshAsset: {}", name);
    }


    MeshAsset::~MeshAsset()
    {
        ReleaseThumbnail();
        Logger::Debug("Destroyed MeshAsset: {}", GetName());
    }


    long MeshAsset::GetSize() const
    {
        if (!m_mesh)
        {
            return 0;
        }

        // 估算网格内存大小
        long size = 0;

        // 顶点数据大小
        size += m_mesh->GetVertices().size() * sizeof(float);

        // 索引数据大小
        size += m_mesh->GetIndices().size() * sizeof(unsigned int);

        return size;
    }

    std::shared_ptr<Mesh> MeshAsset::GetMesh() const
    {
        return m_mesh;
    }

    void MeshAsset::SetMesh(const std::shared_ptr<Mesh> &mesh)
    {
        m_mesh = mesh;
        if (mesh)
        {
            UpdateMeshStatistics();
            GenerateThumbnail();
        }
        else
        {
            m_vertexCount = 0;
            m_triangleCount = 0;
        }
    }

    long MeshAsset::GetVertexCount() const
    {
        return m_vertexCount;
    }

    long MeshAsset::GetTriangleCount() const
    {
        return m_triangleCount;
    }

    rapidjson::Value MeshAsset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue = Asset::Serialize(allocator);

        // 添加MeshAsset特有的属性
        jsonValue.AddMember("vertexCount", static_cast<int64_t>(m_vertexCount), allocator);
        jsonValue.AddMember("triangleCount", static_cast<int64_t>(m_triangleCount), allocator);

        return jsonValue;
    }

    void MeshAsset::Deserialize(const rapidjson::Value &json)
    {
        Asset::Deserialize(json);

        if (json.HasMember("vertexCount") && json["vertexCount"].IsInt64())
        {
            m_vertexCount = json["vertexCount"].GetInt64();
        }

        if (json.HasMember("triangleCount") && json["triangleCount"].IsInt64())
        {
            m_triangleCount = json["triangleCount"].GetInt64();
        }
    }

    void MeshAsset::UpdateMeshStatistics()
    {
        if (!m_mesh)
        {
            m_vertexCount = 0;
            m_triangleCount = 0;
            return;
        }

        // 计算顶点数量（假设每个顶点有8个浮点数：3位置+3法线+2UV）
        m_vertexCount = m_mesh->GetVertices().size() / 8;

        // 计算三角形数量
        if (m_mesh->GetIndices().empty())
        {
            m_triangleCount = m_vertexCount / 3;
        }
        else
        {
            m_triangleCount = m_mesh->GetIndices().size() / 3;
        }
    }

    void MeshAsset::GenerateThumbnail()
    {
        if (!m_mesh)
        {
            return;
        }

        Logger::Debug("not Generated thumbnail for mesh: {}", GetName());
    }

} // namespace framework
