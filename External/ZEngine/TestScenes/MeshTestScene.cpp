#include "TestScenes/MeshTestScene.h"
#include "Framework/Core/Input.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Core/ResLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "Framework/Render/MeshRenderer.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/MeshFilter.h"
#include "Framework/Render/Shader.h"
#include "Framework/Component/Camera.h"
#include "Framework/Manager/CameraManager.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/ObjMeshLoader.h"
#include "Framework/Asset/TextureLoader.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Component/Light/Light.h"
namespace framework
{
    void MeshTestScene::Initialize()
    {
        AssetManager::GetInstance().RegisterLoader(std::make_shared<ObjMeshLoader>());
        AssetManager::GetInstance().RegisterLoader(std::make_shared<TextureLoader>());

        Logger::Log("Initializing Mesh Test Scene");

        // 1. 主相机 - 透视投影
        GameObject *mainCameraObj = new GameObject("MainCamera");

        Camera *mainCamera = mainCameraObj->AddComponent<Camera>();
        mainCamera->SetProjectionType(ProjectionType::Perspective);
        mainCamera->SetPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        mainCamera->SetMainCamera(true);
        CameraManager::GetInstance().RegisterCamera(mainCamera);
        CameraManager::GetInstance().SetMainCamera(mainCamera);

        // 添加相机到场景
        AddGameObject(mainCameraObj);

        auto shader = new Shader("Shaders/BaseLight.vs", "Shaders/BaseLight.fs");
        auto texture = Texture::LoadTexture("Textures/container.png");

        auto maertial = new Material();
        maertial->SetShader(shader);

        // maertial->SetTexture("texture1", 0);

        maertial->SetTexture("texture1", texture, 0, TextureType::DIFFUSE);

        auto meshAsset = std::dynamic_pointer_cast<MeshAsset>(AssetManager::GetInstance().LoadAsset("Models/primitives/Cube.obj", AssetType::Mesh));

        m_cubeObject = new GameObject("Cube");
        auto meshFilter = m_cubeObject->AddComponent<MeshFilter>();
        meshFilter->SetMesh(meshAsset->GetMesh());

        auto meshRenderer = m_cubeObject->AddComponent<MeshRenderer>();
        meshRenderer->SetMaterial(maertial);
        meshRenderer->SetVisible(true);
        m_cubeObject->GetTransform()->SetPosition(glm::vec3(-1.0f, -1.0f, -9.0f));
        m_cubeObject->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

        AddGameObject(m_cubeObject);

        m_lightObject = new GameObject("Light");
        auto light = m_lightObject->AddComponent<Light>();
        m_lightObject->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
        m_lightObject->GetTransform()->SetPosition(glm::vec3(-2.0f, -1.0f, -5.0f));
        m_lightObject->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        light->SetLightType(LightType::Directional);
        light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        light->SetIntensity(1.0f);

        auto lightMeshFilter = m_lightObject->AddComponent<MeshFilter>();
        lightMeshFilter->SetMesh(meshAsset->GetMesh());
        auto lightMeshRenderer = m_lightObject->AddComponent<MeshRenderer>();
        lightMeshRenderer->SetMaterial(maertial);
        AddGameObject(m_lightObject);
    }

    void MeshTestScene::Update(float deltaTime)
    {
        // 使用键盘的wasd 让cube移动
        if (Input::GetKeyDown(GLFW_KEY_W))
        {
            m_cubeObject->GetTransform()->SetPosition(m_cubeObject->GetTransform()->GetPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
        }
        if (Input::GetKeyDown(GLFW_KEY_S))
        {
            m_cubeObject->GetTransform()->SetPosition(m_cubeObject->GetTransform()->GetPosition() - glm::vec3(0.0f, 0.1f, 0.0f));
        }
        if (Input::GetKeyDown(GLFW_KEY_A))
        {
            m_cubeObject->GetTransform()->SetPosition(m_cubeObject->GetTransform()->GetPosition() - glm::vec3(0.1f, 0.0f, 0.0f));
        }
        if (Input::GetKeyDown(GLFW_KEY_D))
        {
            m_cubeObject->GetTransform()->SetPosition(m_cubeObject->GetTransform()->GetPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
        }
        // let cube auto rotate around y axis
        m_cubeObject->GetTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * 50.0f);
    }
    void MeshTestScene::Render(Renderer *renderer)
    {
        Scene::Render(renderer);
    }
    void MeshTestScene::Shutdown()
    {
    }
}
