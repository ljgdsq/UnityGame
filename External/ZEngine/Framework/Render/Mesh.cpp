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
    m_vertexArray->AddLayout({0, 3, GL_FLOAT, false, 5 * sizeof(float), 0});
    m_vertexArray->AddLayout({1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float)}); // 修复：纹理坐标偏移应该是 3 * sizeof(float)
    m_vertexArray->EnableAttributes(0);
    m_vertexArray->EnableAttributes(1);
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
