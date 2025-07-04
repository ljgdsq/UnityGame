#include "TestScenes/TriangleTestScene.h"
#include "Framework/Log/Logger.h"

namespace framework {

void TriangleTestScene::Initialize() {
    Logger::Log("Initializing Triangle Test Scene");
    
    // 创建着色器
    m_shader = new Shader("Shaders/color.vs", "Shaders/color.fs");
    
    // 设置顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    // 创建和配置顶点缓冲区和顶点数组对象
    // glGenVertexArrays(1, &m_VAO);
    // glGenBuffers(1, &m_VBO);
    
    // glBindVertexArray(m_VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // // 解绑缓冲区和顶点数组对象
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // glBindVertexArray(0); 
    VertexArrayObject = new VertexArray();
    VertexBufferObject = new Buffer(BufferType::VBO);
    VertexArrayObject->BindBuffer();
    VertexBufferObject->BindBuffer();
    VertexBufferObject->UpdateData(vertices, sizeof(vertices), BufferUsage::StaticDraw);
    VertexArrayObject->AddLayout({0, 3, GL_FLOAT, false, 3 * sizeof(float), 0});
    VertexArrayObject->EnableAttributes(0);
    VertexArrayObject->UnbindBuffer();
    VertexBufferObject->UnbindBuffer();

}

void TriangleTestScene::Update(float deltaTime) {
    // 这个简单的测试场景不需要更新逻辑
}

void TriangleTestScene::Render(Renderer*) {
    // 使用着色器
    m_shader->Use();
    VertexArrayObject->BindBuffer();
    // 绑定VAO并绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
    VertexArrayObject->UnbindBuffer();
}

void TriangleTestScene::Shutdown() {
    Logger::Log("Shutting down Triangle Test Scene");

    // 清理资源
    VertexArrayObject->Destroy();
    VertexBufferObject->Destroy();
    
    if (m_shader) {
        delete m_shader;
        m_shader = nullptr;
    }
}

} // namespace framework
