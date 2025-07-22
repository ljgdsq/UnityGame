#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Graphic/Mesh.h"
#include "Framework/Log/Logger.h"
#include <memory>
#include <string>
#include <vector>

namespace framework
{
    enum class MeshPrimitive
    {
        Cube,
        Sphere,
        Plane,
        Cylinder,
        Capsule
    };

    class MeshAsset : public Asset
    {
    public:
        MeshAsset(const std::string &name);
        MeshAsset(const std::string &name, const std::string &assetId);
        virtual ~MeshAsset() override;

        long GetSize() const override;

        // Serialization
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        void Deserialize(const rapidjson::Value &json) override;

        // 网格数据
        std::shared_ptr<Mesh> GetMesh() const;
        void SetMesh(const std::shared_ptr<Mesh> &mesh);

        // 统计信息
        long GetVertexCount() const;
        long GetTriangleCount() const;

    private:

        // 辅助方法
        void UpdateMeshStatistics();
        void GenerateThumbnail() override;

    private:
        std::shared_ptr<Mesh> m_mesh; // 网格数据
        long m_vertexCount = 0;       // 顶点数量
        long m_triangleCount = 0;     // 三角形数量
    };
} // namespace framework
