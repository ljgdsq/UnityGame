#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Graphic/Shader.h"
#include <glad/glad.h>
#include "Framework/Graphic/Buffer.h"
namespace framework
{

    class QuadTestScene : public Scene
    {
    private:
        Shader *m_shader = nullptr;
        VertexArray *VertexArrayObject = nullptr;
        Buffer *VertexBufferObject = nullptr;
        Buffer *IndexBufferObject = nullptr;

    public:
        QuadTestScene() = default;
        ~QuadTestScene() override = default;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(Renderer *) override;
        void Shutdown() override;

        const char *GetName() const override { return "QuadTestScene"; }
    };

} // namespace framework
