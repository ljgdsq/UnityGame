#include "BasicTestScene.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"

namespace framework
{

    void BasicTestScene::Initialize()
    {
        LOG_INFO("Initializing BasicTestScene");

        // 创建着色器
//        m_shader = new Shader("Shaders/CameraExample.vs", "Shaders/CameraExample.fs");

        auto asset=AssetManager::GetInstance().LoadAsset<TextureAsset>("Textures/container.png");

    }

    void BasicTestScene::Update(float deltaTime)
    {

    }

    void BasicTestScene::Render(Renderer *renderer)
    {

    }

    void BasicTestScene::Shutdown()
    {
        LOG_INFO("Shutting down BasicTestScene");
    }


} // namespace framework
