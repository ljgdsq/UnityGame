#include "TestScenes/MeshTestScene.h"
#include "Framework/Core/Input.h"
#include "Framework/Log/Logger.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Core/ResLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glfw/glfw3.h"
#include "Framework/Render/MeshRenderer.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/MeshFilter.h"
#include "Framework/Render/Shader.h"
#include "Framework/Component/Camera.h"
#include "Framework/Manager/CameraManager.h"
namespace framework
{
    void MeshTestScene::Initialize()
    {
        Logger::Log("Initializing Mesh Test Scene");

            // 1. 主相机 - 透视投影
    GameObject* mainCameraObj = new GameObject();

        Camera* mainCamera = mainCameraObj->AddComponent<Camera>();
    mainCamera->SetProjectionType(ProjectionType::Perspective);
    mainCamera->SetPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    mainCamera->SetMainCamera(true);        CameraManager::GetInstance().RegisterCamera(mainCamera);
        CameraManager::GetInstance().SetMainCamera(mainCamera);
        
        // 添加相机到场景
        AddGameObject(mainCameraObj);

        auto shader = new Shader("Shaders/CameraExample.vs", "Shaders/CameraExample.fs");

        auto texture = Texture::LoadTexture("Textures/container.png");

        auto maertial= new Material();
        maertial->SetShader(shader);

        maertial->SetTexture("texture1", 0);
 float vertices[] = {
        // 位置                 // 纹理坐标
        // 前面
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        
        // 后面
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        
        // 左面
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        
        // 右面
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        
        // 上面
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        
        // 下面
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f
    };
        // 立方体索引数据
    unsigned int indices[] = {
        // 前面
        0, 1, 2,
        2, 3, 0,
        
        // 后面
        4, 5, 6,
        6, 7, 4,
        
        // 左面
        8, 9, 10,
        10, 11, 8,
        
        // 右面
        12, 13, 14,
        14, 15, 12,
        
        // 上面
        16, 17, 18,
        18, 19, 16,
        
        // 下面
        20, 21, 22,
        22, 23, 20
    };
        auto mesh = std::make_unique<Mesh>();
        mesh->SetVertices(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float)));
        mesh->SetIndices(std::vector<unsigned int>(indices, indices + sizeof(indices) / sizeof(unsigned int)));


        m_cubeObject = new GameObject("Cube");
        auto meshFilter = m_cubeObject->AddComponent<MeshFilter>();
        meshFilter->SetMesh(std::move(mesh));

        auto meshRenderer = m_cubeObject->AddComponent<MeshRenderer>();
        meshRenderer->SetMaterial(maertial);
        meshRenderer->SetVisible(true);
        m_cubeObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        m_cubeObject->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

        AddGameObject(m_cubeObject);


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
        
    }
    void MeshTestScene::Render(Renderer *renderer)
    {
        Scene::Render(renderer);
    }
    void MeshTestScene::Shutdown()
    {
    }
}