#include "BasicTestScene.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Graphic/MeshRenderer.h"
#include "Framework/Graphic/MeshFilter.h"
#include "Framework/Graphic/Material.h"

#include "Framework/Component/Light/Light.h"
#include "Framework/Manager/LightManager.h"
namespace framework
{

    void BasicTestScene::Initialize()
    {
        LOG_INFO("Initializing BasicTestScene");

        // 创建着色器
        auto shader = new Shader("Shaders/BaseLight.vs", "Shaders/BaseLight.fs");

        auto texture = AssetManager::GetInstance().LoadAsset<TextureAsset>("Textures/container.png");
        auto material = new Material("TestMaterial");
        material->SetShader(shader);
        material->SetTexture("texture_1", texture, 0, TextureType::DIFFUSE);

        auto go = new GameObject("TestTexture");
        go->AddComponent<MeshFilter>();
        go->GetComponent<MeshFilter>()->SetMesh(AssetManager::GetInstance().LoadAsset<MeshAsset>("Models/primitives/Cube.obj"));

        go->AddComponent<MeshRenderer>();
        go->GetComponent<MeshRenderer>()->SetMaterial(material);

        auto light = new GameObject("TestLight");
        light->AddComponent<Light>();
        light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 0.5f, 0.2f));
        light->GetComponent<Light>()->SetIntensity(1.0f);
        light->GetTransform()->SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));

        LightManager::RegisterLight(light->GetComponent<Light>());

        auto camera = new GameObject("MainCamera");
        camera->AddComponent<Camera>();
        camera->GetComponent<Camera>()->SetMainCamera(true);
        camera->GetComponent<Camera>()->SetProjectionType(ProjectionType::Perspective);
        camera->GetComponent<Camera>()->SetPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        camera->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        CameraManager::GetInstance().RegisterCamera(camera->GetComponent<Camera>());
    }

    void BasicTestScene::Update(float deltaTime)
    {
    }

    void BasicTestScene::Render(Renderer *renderer)
    {
        Scene::Render(renderer);
    }

    void BasicTestScene::Shutdown()
    {
        LOG_INFO("Shutting down BasicTestScene");
    }

} // namespace framework
