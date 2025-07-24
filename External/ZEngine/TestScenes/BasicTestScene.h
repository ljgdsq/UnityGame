#pragma once

#include "Framework/Core/Scene.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Camera.h"
#include "Framework/Component/Transform.h"
#include "Framework/Manager/CameraManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Core/Texture.h"
#include <vector>

namespace framework
{

    /**
     * @brief 演示如何使用CameraManager的示例场景
     * 这个场景创建了多个相机，并展示了如何在它们之间切换
     */

    class BasicTestScene : public Scene
    {
    public:
        BasicTestScene() = default;
        ~BasicTestScene() override = default;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(Renderer *renderer) override;
        void Shutdown() override;

    };

} // namespace framework
