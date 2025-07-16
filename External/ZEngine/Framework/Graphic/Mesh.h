#pragma once
#include <vector>
#include "Framework/Graphic/Buffer.h"
namespace framework
{
    class GameObject;
    class Transform;

    /**
     * @brief 网格类
     * 用于表示3D模型的网格数据
     */
    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

        // 设置顶点数据
        void SetVertices(const std::vector<float> &vertices);

        // 设置索引数据
        void SetIndices(const std::vector<unsigned int> &indices);

        // 获取顶点数据
        const std::vector<float> &GetVertices() const { return vertices; }

        // 获取索引数据
        const std::vector<unsigned int> &GetIndices() const { return indices; }

    public:
        // 使用网格数据
        void Use();

    private:
        std::vector<float> vertices;       // 顶点数据
        std::vector<unsigned int> indices; // 索引数据

        VertexArray *m_vertexArray = nullptr; // 顶点数组对象
        Buffer *m_vertexBuffer = nullptr;     // 顶点缓冲区
        Buffer *m_indexBuffer = nullptr;      // 索引缓冲区
    };
} // namespace framework
