#include "CameraManagerExampleScene.h"
#include "Framework/Core/Input.h"
#include "Framework/Log/Logger.h"
#include "Framework/Graphic/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
namespace framework
{

    void CameraManagerExampleScene::Initialize()
    {
        LOG_INFO("Initializing CameraManagerExampleScene");

        // 创建着色器
        m_shader = new Shader("Shaders/CameraExample.vs", "Shaders/CameraExample.fs");

        // 创建纹理
        m_cubeTexture = Texture::LoadTexture("Textures/container.png");

        // 创建立方体
        CreateCube();

        // 创建相机
        CreateCameras();

        // 初始化相机管理器，确保第一个相机是主相机
        if (!m_cameras.empty())
        {
            CameraManager::GetInstance().SetMainCamera(m_cameras[0]);
            m_activeCameraIndex = 0;
            LOG_INFO("Set camera {} as main camera", 0);
        }
    }

    void CameraManagerExampleScene::Update(float deltaTime)
    {
        // 更新相机定时器
        m_cameraTimer += deltaTime;

        // 如果按下空格键，切换到下一个相机
        if (Input::GetKeyDown(GLFW_KEY_F10))
        {
            SwitchToNextCamera();
            m_cameraTimer = 0.0f; // 重置定时器
        }

        // 定时自动切换相机
        if (m_cameraTimer >= m_cameraSwitchInterval)
        {
            SwitchToNextCamera();
            m_cameraTimer = 0.0f; // 重置定时器
        }

        // 旋转立方体
        for (auto gameObject : m_gameObjects)
        {
            auto transform = gameObject->GetComponent<Transform>();
            if (transform)
            {
                transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * 45.0f);
            }
        }
    }

    void CameraManagerExampleScene::Render(Renderer *renderer)
    {
        // 获取主相机
        Camera *mainCamera = CameraManager::GetInstance().GetMainCamera();
        if (!mainCamera)
        {
            LOG_ERROR("No main camera found!");
            return;
        }

        // 清除背景色
        glClearColor(mainCamera->clearColor.r, mainCamera->clearColor.g,
                     mainCamera->clearColor.b, mainCamera->clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 启用深度测试
        glEnable(GL_DEPTH_TEST);

        // 使用着色器程序
        m_shader->Use();

        // 绑定纹理
        m_cubeTexture->Bind(0);
        m_shader->SetInt("texture1", 0);

        // 设置相机视图和投影矩阵
        m_shader->SetMat4("view", mainCamera->GetViewMatrix());
        m_shader->SetMat4("projection", mainCamera->GetProjectionMatrix());

        // 渲染立方体
        m_cubeVAO->BindBuffer();
        for (auto gameObject : m_gameObjects)
        {
            auto transform = gameObject->GetComponent<Transform>();
            if (transform)
            {
                // 设置模型矩阵
                m_shader->SetMat4("model", transform->GetModelMatrix());

                // 绘制立方体
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        // 解绑VAO
        m_cubeVAO->UnbindBuffer();
    }

    void CameraManagerExampleScene::Shutdown()
    {
        LOG_INFO("Shutting down CameraManagerExampleScene");

        // 清理相机管理器
        CameraManager::GetInstance().Clear();

        // 删除着色器
        delete m_shader;
        m_shader = nullptr;

        // 删除纹理
        delete m_cubeTexture;
        m_cubeTexture = nullptr;

        // 删除VAO和VBO
        delete m_cubeVAO;
        m_cubeVAO = nullptr;

        delete m_cubeVBO;
        m_cubeVBO = nullptr;

        delete m_cubeEBO;
        m_cubeEBO = nullptr;

        // 删除游戏对象
        for (auto gameObject : m_gameObjects)
        {
            delete gameObject;
        }
        m_gameObjects.clear();

        // 清除相机列表（相机已经通过游戏对象删除）
        m_cameras.clear();
    }

    void CameraManagerExampleScene::CreateCube()
    {
        // 立方体顶点数据（位置和纹理坐标）
        float vertices[] = {
            // 位置                 // 纹理坐标
            // 前面
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

            // 后面
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

            // 左面
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

            // 右面
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

            // 上面
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

            // 下面
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f};

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
            22, 23, 20};

        // 创建VAO、VBO和EBO
        m_cubeVAO = new VertexArray();
        m_cubeVBO = new Buffer(BufferType::VBO);
        m_cubeEBO = new Buffer(BufferType::EBO);

        m_cubeVAO->BindBuffer();

        // 绑定并设置VBO
        m_cubeVBO->BindBuffer();
        m_cubeVBO->UpdateData(vertices, sizeof(vertices));

        // 绑定并设置EBO
        m_cubeEBO->BindBuffer();
        m_cubeEBO->UpdateData(indices, sizeof(indices));

        // 设置顶点属性
        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // 纹理坐标属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 创建多个立方体
        for (int i = 0; i < 5; ++i)
        {
            GameObject *cube = new GameObject();
            auto transform = cube->GetTransform();

            // 设置立方体位置
            float angle = i * (360.0f / 5);
            float radius = 3.0f;
            float x = radius * cos(glm::radians(angle));
            float z = radius * sin(glm::radians(angle));

            transform->SetPosition(glm::vec3(x, 0.0f, z));
            transform->SetScale(glm::vec3(1.0f));

            // 随机旋转
            transform->SetRotation(glm::vec3(0.0f, angle, 0.0f));

            m_gameObjects.push_back(cube);
        }

        // 解绑VAO
        m_cubeVAO->UnbindBuffer();
    }

    void CameraManagerExampleScene::CreateCameras()
    {
        // 创建四个不同位置和投影类型的相机

        // 1. 主相机 - 透视投影
        GameObject *mainCameraObj = new GameObject();
        auto mainCameraTransform = mainCameraObj->GetTransform();
        mainCameraTransform->SetPosition(glm::vec3(0.0f, 2.0f, 8.0f));
        mainCameraTransform->SetRotation(glm::vec3(-15.0f, 0.0f, 0.0f));

        Camera *mainCamera = mainCameraObj->AddComponent<Camera>();
        mainCamera->SetProjectionType(ProjectionType::Perspective);
        mainCamera->SetPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        mainCamera->clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
        mainCamera->SetMainCamera(true);

        m_cameras.push_back(mainCamera);
        m_gameObjects.push_back(mainCameraObj);
        CameraManager::GetInstance().RegisterCamera(mainCamera);

        // 2. 侧面相机 - 透视投影
        GameObject *sideCameraObj = new GameObject();
        auto sideCameraTransform = sideCameraObj->GetTransform();
        sideCameraTransform->SetPosition(glm::vec3(8.0f, 2.0f, 0.0f));
        sideCameraTransform->SetRotation(glm::vec3(-15.0f, -90.0f, 0.0f));

        Camera *sideCamera = sideCameraObj->AddComponent<Camera>();
        sideCamera->SetProjectionType(ProjectionType::Perspective);
        sideCamera->SetPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        sideCamera->clearColor = glm::vec4(0.3f, 0.2f, 0.3f, 1.0f);

        m_cameras.push_back(sideCamera);
        m_gameObjects.push_back(sideCameraObj);
        CameraManager::GetInstance().RegisterCamera(sideCamera);

        // 3. 顶部相机 - 正交投影
        GameObject *topCameraObj = new GameObject();
        auto topCameraTransform = topCameraObj->GetTransform();
        topCameraTransform->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
        topCameraTransform->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

        Camera *topCamera = topCameraObj->AddComponent<Camera>();
        topCamera->SetProjectionType(ProjectionType::Orthographic);
        topCamera->SetOrthographicProjection(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

        m_cameras.push_back(topCamera);
        m_gameObjects.push_back(topCameraObj);
        CameraManager::GetInstance().RegisterCamera(topCamera);

        // 4. 远距离相机 - 透视投影
        GameObject *farCameraObj = new GameObject();
        auto farCameraTransform = farCameraObj->GetTransform();
        farCameraTransform->SetPosition(glm::vec3(0.0f, 2.0f, 15.0f));
        farCameraTransform->SetRotation(glm::vec3(-10.0f, 0.0f, 0.0f));

        Camera *farCamera = farCameraObj->AddComponent<Camera>();
        farCamera->SetProjectionType(ProjectionType::Perspective);
        farCamera->SetPerspectiveProjection(30.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        farCamera->clearColor = glm::vec4(0.1f, 0.1f, 0.3f, 1.0f);

        m_cameras.push_back(farCamera);
        m_gameObjects.push_back(farCameraObj);
        CameraManager::GetInstance().RegisterCamera(farCamera);
    }

    void CameraManagerExampleScene::SwitchToNextCamera()
    {
        if (m_cameras.empty())
        {
            return;
        }

        // 计算下一个相机的索引
        m_activeCameraIndex = (m_activeCameraIndex + 1) % m_cameras.size();

        // 设置新的主相机
        Camera *nextCamera = m_cameras[m_activeCameraIndex];
        CameraManager::GetInstance().SetMainCamera(nextCamera);

        LOG_INFO("Switched to camera {}", m_activeCameraIndex);
    }

} // namespace framework
