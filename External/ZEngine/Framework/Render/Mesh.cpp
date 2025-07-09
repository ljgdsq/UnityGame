#include "Mesh.h"

void framework::Mesh::SetVertices(const std::vector<float> &vertices)
{
    this->vertices = vertices;

    // 创建或更新顶点缓冲区
    if (!m_vertexBuffer)
    {
        m_vertexBuffer = new Buffer(BufferType::VBO);
    }
    m_vertexBuffer->BindBuffer();
    m_vertexBuffer->UpdateData(vertices.data(), vertices.size() * sizeof(float), BufferUsage::StaticDraw);

    // 更新顶点数组对象
    if (!m_vertexArray)
    {
        m_vertexArray = new VertexArray();
    }
    m_vertexArray->BindBuffer();
    
    // 检测顶点数据格式（根据顶点分量数量）
    // 如果是8个分量：position(3) + normal(3) + texCoord(2)
    // 如果是5个分量：position(3) + texCoord(2)
    
    if (vertices.size() % 8 == 0) {
        // 8分量格式：position(3) + normal(3) + texCoord(2)
        m_vertexArray->AddLayout({0, 3, GL_FLOAT, false, 8 * sizeof(float), 0}); // position
        m_vertexArray->AddLayout({1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float)}); // normal
        m_vertexArray->AddLayout({2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float)}); // texCoord
        m_vertexArray->EnableAttributes(0);
        m_vertexArray->EnableAttributes(1);
        m_vertexArray->EnableAttributes(2);
    } else if (vertices.size() % 5 == 0) {
        // 5分量格式：position(3) + texCoord(2)
        m_vertexArray->AddLayout({0, 3, GL_FLOAT, false, 5 * sizeof(float), 0}); // position
        m_vertexArray->AddLayout({1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float)}); // texCoord
        m_vertexArray->EnableAttributes(0);
        m_vertexArray->EnableAttributes(1);
    } else {
        // 默认只有位置数据（3分量）
        m_vertexArray->AddLayout({0, 3, GL_FLOAT, false, 3 * sizeof(float), 0}); // position
        m_vertexArray->EnableAttributes(0);
    }
    
    m_vertexArray->UnbindBuffer();
}

void framework::Mesh::Use()
{
    if (m_vertexArray)
    {
        m_vertexArray->BindBuffer();
    }
    if (m_vertexBuffer)
    {
        m_vertexBuffer->BindBuffer();
    }
    if (m_indexBuffer)
    {
        m_indexBuffer->BindBuffer();
    }
}

void framework::Mesh::SetIndices(const std::vector<unsigned int> &indices)
{
    this->indices = indices;
    
    // 绑定 VAO 以确保 EBO 与 VAO 关联
    if (m_vertexArray)
    {
        m_vertexArray->BindBuffer();
    }
    
    // 创建或更新索引缓冲区
    if (!m_indexBuffer)
    {
        m_indexBuffer = new Buffer(BufferType::EBO);
    }
    m_indexBuffer->BindBuffer();
    m_indexBuffer->UpdateData(indices.data(), indices.size() * sizeof(unsigned int), BufferUsage::StaticDraw);
    
    // 解绑 VAO，EBO 会自动与 VAO 关联
    if (m_vertexArray)
    {
        m_vertexArray->UnbindBuffer();
    }
}
