#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Render/Mesh.h"
#include <memory>
#include <string>

namespace framework
{
    class MeshAsset : public Asset
    {
    public:
        MeshAsset(const std::string &name);
        virtual ~MeshAsset() override;

        long GetSize() const override;

        // 网格数据
        std::shared_ptr<Mesh> GetMesh() const;
        // 设置网格数据
        void SetMesh(const std::shared_ptr<Mesh> &mesh);
        // 获取顶点数量
        long GetVertexCount() const;
        // 获取三角形数量
        long GetTriangleCount() const;

    private:
        std::shared_ptr<Mesh> m_mesh; // 网格数据
        long m_vertexCount = 0; // 顶点数量
        long m_triangleCount = 0; // 三角形数量
    };
} // namespace framework