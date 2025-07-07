#include "Framework/Asset/MeshAsset.h"

namespace framework
{
    MeshAsset::MeshAsset(const std::string &name)
        : Asset(name, AssetType::Mesh)
    {
    }

    ~MeshAsset::~MeshAsset()
    {
        // 资源卸载时可以在这里添加清理逻辑
    }

    long MeshAsset::GetSize() const
    {
        return m_mesh ? m_mesh->GetSize() : 0;
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
            m_vertexCount = m_mesh->GetVertexCount();
            m_triangleCount = m_mesh->GetTriangleCount();
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