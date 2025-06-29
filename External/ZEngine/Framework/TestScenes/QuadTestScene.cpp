#include "Framework/TestScenes/QuadTestScene.h"
#include "Framework/Log/Logger.h"

namespace framework {

void QuadTestScene::Initialize() {
    Logger::Log("Initializing Quad Test Scene");
    
    // 创建着色器
    m_shader = new Shader("Shaders/color.vs", "Shaders/color.fs");
    
    // 设置顶点数据（一个正方形，由两个三角形组成）
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    // 创建和配置顶点缓冲区、索引缓冲区和顶点数组对象
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 解绑缓冲区和顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void QuadTestScene::Update(float deltaTime) {
    // 这个简单的测试场景不需要更新逻辑
}

void QuadTestScene::Render() {
    // 使用着色器
    m_shader->Use();
    
    // 绑定VAO并绘制正方形
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void QuadTestScene::Shutdown() {
    Logger::Log("Shutting down Quad Test Scene");
    
    // 清理资源
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    
    if (m_shader) {
        delete m_shader;
        m_shader = nullptr;
    }
}

} // namespace framework
