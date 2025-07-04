#include "TextureScene.h"
#include "Framework/Core/Texture.h"
void TextureScene::Initialize()
{
    Logger::Log("Initializing TextureScene");
    shader = new Shader("Shaders/Texture.vs", "Shaders/Texture.fs");
    float vertices[] = {
        // 位置              // 纹理坐标    
        0.5f, 0.5f, 0.0f,    1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // 左下
        -0.5f, 0.5f, 0.0f,   0.0f, 1.0f    // 左上
    };

        unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    vertexArrayObject = new VertexArray();
    vertexBufferObject = new Buffer(BufferType::VBO);
    indexBufferObject = new Buffer(BufferType::EBO);
    vertexArrayObject->BindBuffer();
    vertexBufferObject->BindBuffer();
    indexBufferObject->BindBuffer();
    vertexBufferObject->UpdateData(vertices, sizeof(vertices));
    indexBufferObject->UpdateData(indices, sizeof(indices));

    vertexArrayObject->AddLayout({0, 3, GL_FLOAT, false, 5 * sizeof(float), 0});
    vertexArrayObject->EnableAttributes(0);
    vertexArrayObject->AddLayout({1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float)});
    vertexArrayObject->EnableAttributes(1);

    vertexBufferObject->UnbindBuffer();
    vertexArrayObject->UnbindBuffer();

    texture = framework::Texture::LoadTexture("Textures/face.png");

}

void TextureScene::Update(float deltaTime)
{
}

void TextureScene::Render(Renderer*)
{
    shader->Use();
    texture->Bind();
    shader->SetInteger("texture1", 0);
    vertexArrayObject->BindBuffer();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TextureScene::Shutdown()
{
}
