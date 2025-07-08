#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Log/Logger.h"
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


        // 重载 Asset 的方法
        void Load() override {

            Logger::Error("MeshAsset::Load is not implemented yet");

        }
        void Unload() override {
            Logger::Error("MeshAsset::Unload is not implemented yet");
        }


    private:
        std::shared_ptr<Mesh> m_mesh; // 网格数据
        long m_vertexCount = 0; // 顶点数量
        long m_triangleCount = 0; // 三角形数量
    };
} // namespace framework