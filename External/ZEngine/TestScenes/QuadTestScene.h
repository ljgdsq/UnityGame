#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Render/Shader.h"
#include <glad/glad.h>

namespace framework {

class QuadTestScene : public Scene {
private:
    Shader* m_shader = nullptr;
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;  // 索引缓冲对象

public:
    QuadTestScene() = default;
    ~QuadTestScene() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;

    const char* GetName() const override { return "QuadTestScene"; }
};

} // namespace framework
