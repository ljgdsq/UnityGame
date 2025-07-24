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

    class CameraManagerExampleScene : public Scene
    {
    public:
        CameraManagerExampleScene() = default;
        ~CameraManagerExampleScene() override = default;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(Renderer *renderer) override;
        void Shutdown() override;

    private:
        // 创建一个立方体对象用于演示不同相机视角
        void CreateCube();

        // 创建各种不同的相机
        void CreateCameras();

        // 切换到下一个相机
        void SwitchToNextCamera();

    private:
        // 场景中的游戏对象
        std::vector<GameObject *> m_gameObjects;

        // 场景中的相机对象
        std::vector<Camera *> m_cameras;

        // 当前活动相机的索引
        int m_activeCameraIndex = 0;

        // 累积时间，用于定时切换相机
        float m_cameraTimer = 0.0f;

        // 相机切换时间间隔（秒）
        float m_cameraSwitchInterval = 5.0f;

        // 渲染资源
        Shader *m_shader = nullptr;
        VertexArray *m_cubeVAO = nullptr;
        Buffer *m_cubeVBO = nullptr;
        Buffer *m_cubeEBO = nullptr;
        Texture *m_cubeTexture = nullptr;
    };

} // namespace framework
