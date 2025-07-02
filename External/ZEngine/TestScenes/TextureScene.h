#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Render/Shader.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Core/Texture.h"
using namespace framework;
class TextureScene : public framework::Scene
{
public:
    TextureScene() = default;
    ~TextureScene() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;

    const char *GetName() const override { return "TextureScene"; }

private:
    Shader *shader = nullptr;
     Texture *texture = nullptr;
    VertexArray *vertexArrayObject = nullptr;
    Buffer *vertexBufferObject = nullptr;
    Buffer *indexBufferObject = nullptr;
};