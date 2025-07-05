#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Camera.h"
#include "Framework/Component/Transform.h"
#include "Framework/Manager/CameraManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Shader.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Core/Texture.h"
#include <vector>


namespace framework
{
    class MeshTestScene : public Scene
    {
    public:
        MeshTestScene() = default;
        ~MeshTestScene() override = default;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(Renderer* renderer) override;
        void Shutdown() override;

        const char* GetName() const override { return "MeshTestScene"; }
        private:
            // 场景中的游戏对象
            GameObject* m_cubeObject = nullptr;

    };
} // namespace framework
