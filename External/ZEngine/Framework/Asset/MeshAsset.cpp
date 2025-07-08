#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Mesh.h"
namespace framework
{
    MeshAsset::MeshAsset(const std::string &name)
        : Asset(name, AssetType::Mesh)
    {
    }

    MeshAsset::~MeshAsset()
    {
    }
    long MeshAsset::GetSize() const
    {
        if (m_mesh)
        {
            return m_mesh->GetVertices().size() * sizeof(float) + m_mesh->GetIndices().size() * sizeof(unsigned int);
        }
        return 0; // 如果没有网格数据，返回0        
    }

    std::shared_ptr<Mesh> MeshAsset::GetMesh() const
    {
        return m_mesh;
    }
    void MeshAsset::SetMesh(const std::shared_ptr<Mesh> &mesh)
    {
        m_mesh = mesh;
        if (m_mesh)
        {
            Logger::Error("MeshAsset::SetMesh: m_mesh->GetVertices().size() / 3 may error");
            m_vertexCount = m_mesh->GetVertices().size() / 3; // 假设每个顶点有3个分量（x, y, z）
            m_triangleCount = m_mesh->GetIndices().size() / 3; //
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

}