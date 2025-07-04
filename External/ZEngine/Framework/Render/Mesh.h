#pragma once
#include <vector>
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
        const std::vector<float>& GetVertices() const { return vertices; }
        
        // 获取索引数据
        const std::vector<unsigned int>& GetIndices() const { return indices; }

    private:
        std::vector<float> vertices; // 顶点数据
        std::vector<unsigned int> indices; // 索引数据
    };
} // namespace framework

