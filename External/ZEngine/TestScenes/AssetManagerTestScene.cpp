#include "TestScenes/AssetManagerTestScene.h"
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
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/ObjMeshLoader.h"
#include "Framework/Render/Renderer.h"

namespace framework
{
    void AssetManagerTestScene::Initialize()
    {

        AssetManager::GetInstance().RegisterLoader(std::make_shared<ObjMeshLoader>());

        Logger::Log("=== Initializing AssetManager Mesh Test Scene ===");
        
        // 1. 创建主相机
        GameObject* mainCameraObj = new GameObject();
        mainCameraObj->SetName("MainCamera");
        
        Camera* mainCamera = mainCameraObj->AddComponent<Camera>();
        mainCamera->SetProjectionType(ProjectionType::Perspective);
        mainCamera->SetPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
        mainCamera->SetMainCamera(true);
        
        CameraManager::GetInstance().RegisterCamera(mainCamera);
        CameraManager::GetInstance().SetMainCamera(mainCamera);
        
        // 设置相机位置
        mainCameraObj->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));
        
        AddGameObject(mainCameraObj);
        
        // 2. 测试MeshAsset管道
        TestMeshLoaderRegistration();
        TestObjMeshLoading();
        TestMeshAssetCreationAndManagement();
        TestMeshFilterIntegration();
        
        // // 3. 创建使用加载的网格资产的游戏对象
        // CreateMeshObjectsFromAssets();
        
        // 4. 显示操作说明
        Logger::Log("=== MeshAsset Test Scene Controls ===");
        Logger::Log("1 - Test ObjMeshLoader Registration");
        Logger::Log("2 - Test Obj Mesh Loading");
        Logger::Log("3 - Test MeshAsset Management");
        Logger::Log("4 - Test MeshFilter Integration");
        Logger::Log("5 - Test Mesh Memory Management");
        Logger::Log("6 - Print Mesh Asset Statistics");
        Logger::Log("U - Unload All Mesh Assets");
        Logger::Log("R - Reload All Mesh Assets");
        Logger::Log("WASD - Move camera");
        Logger::Log("=== ====================== ===");
    }

    void AssetManagerTestScene::Update(float deltaTime)
    {
        m_testTimer += deltaTime;
        m_uiUpdateTimer += deltaTime;
        
        // 处理测试输入
        HandleTestInput();
        
        // 定期更新显示信息
        if (m_uiUpdateTimer >= m_uiUpdateInterval) {
            TestMeshAssetStatistics();
            m_uiUpdateTimer = 0.0f;
        }
        
        // 让使用网格的对象旋转
        for (auto& obj : m_testObjects) {
            if (obj && obj->IsActive()) {
                auto transform = obj->GetTransform();
                if (transform) {
                    transform->Rotate(glm::vec3(0.0f, 1.0f, 0.3f), deltaTime * 30.0f);
                }
            }
        }
        
        // 相机控制
        auto mainCamera = CameraManager::GetInstance().GetMainCamera();
        if (mainCamera) {
            auto cameraTransform = mainCamera->GetGameObject()->GetTransform();
            glm::vec3 position = cameraTransform->GetPosition();
            
            if (Input::GetKey(GLFW_KEY_W)) {
                position.z -= deltaTime * 2.0f;
            }
            if (Input::GetKey(GLFW_KEY_S)) {
                position.z += deltaTime * 2.0f;
            }
            if (Input::GetKey(GLFW_KEY_A)) {
                position.x -= deltaTime * 2.0f;
            }
            if (Input::GetKey(GLFW_KEY_D)) {
                position.x += deltaTime * 2.0f;
            }
            
            cameraTransform->SetPosition(position);
        }
    }

    void AssetManagerTestScene::Render(Renderer* renderer)
    {
        renderer->EnableDepthTest();
        // 基础场景渲染由基类处理
        Scene::Render(renderer);
    }

    void AssetManagerTestScene::Shutdown()
    {
        Logger::Log("=== Shutting down AssetManager Mesh Test Scene ===");
        
        // 清理测试对象
        m_testObjects.clear();

    }

    void AssetManagerTestScene::TestMeshLoaderRegistration()
    {
        Logger::Log("=== Testing MeshLoader Registration ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 注册ObjMeshLoader
        auto objLoader = std::make_shared<ObjMeshLoader>();
        assetManager.RegisterLoader(objLoader);
        
        Logger::Log("ObjMeshLoader registered successfully");
        
        // 验证loader能够处理.obj文件
        auto supportedExtensions = objLoader->GetSupportedExtensions();
        Logger::Log("ObjMeshLoader supports {} extensions:", supportedExtensions.size());
        for (const auto& ext : supportedExtensions) {
            Logger::Log("  - {}", ext);
        }
        
        Logger::Log("=== MeshLoader Registration Complete ===");
    }

    void AssetManagerTestScene::TestObjMeshLoading()
    {
        Logger::Log("=== Testing Obj Mesh Loading ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 尝试加载每个测试网格文件
        for (const auto& meshPath : m_testMeshPaths) {
            Logger::Log("Attempting to load mesh: {}", meshPath);

            auto asset = assetManager.LoadAsset(meshPath, AssetType::Mesh);
            if (asset) {
                auto meshAsset = std::dynamic_pointer_cast<MeshAsset>(asset);
                if (meshAsset) {
                    Logger::Log("Successfully loaded MeshAsset: {}", meshAsset->GetName());
                    Logger::Log("  - Vertex count: {}", meshAsset->GetVertexCount());
                    Logger::Log("  - Triangle count: {}", meshAsset->GetTriangleCount());
                    Logger::Log("  - Size: {} bytes", meshAsset->GetSize());
                } else {
                    Logger::Warn("Asset is not a MeshAsset: {}", meshPath);
                }
            } else {
                Logger::Warn("Failed to load mesh asset: {}", meshPath);
            }
        }
        
        Logger::Log("=== Obj Mesh Loading Complete ===");
    }

    void AssetManagerTestScene::TestMeshAssetCreationAndManagement()
    {
        Logger::Log("=== Testing MeshAsset Creation and Management ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 测试创建简单的网格资产
        auto meshAsset = std::make_shared<MeshAsset>("TestCube");
        
        // 创建简单立方体网格数据
        std::vector<float> vertices = {
            // 前面
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        };
        
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0
        };
        
        auto mesh = std::make_shared<Mesh>();
        mesh->SetVertices(vertices);
        mesh->SetIndices(indices);
        
        meshAsset->SetMesh(mesh);
        
        Logger::Log("Created MeshAsset: {}", meshAsset->GetName());
        Logger::Log("  - Vertex count: {}", meshAsset->GetVertexCount());
        Logger::Log("  - Triangle count: {}", meshAsset->GetTriangleCount());
        
        // // 添加到AssetManager中进行管理
        // assetManager.AddAsset("TestCube", meshAsset);
        
        Logger::Log("=== MeshAsset Creation and Management Complete ===");
    }

    void AssetManagerTestScene::TestMeshFilterIntegration()
    {
        Logger::Log("=== Testing MeshFilter Integration ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 获取一个已加载的网格资产
        auto asset = assetManager.GetAsset("Cube");
        if (asset) {
            auto meshAsset = std::dynamic_pointer_cast<MeshAsset>(asset);
            if (meshAsset) {
                // 创建游戏对象并添加MeshFilter
                GameObject* testObj = new GameObject();
                testObj->SetName("MeshFilterTestObject");
                
                auto meshFilter = testObj->AddComponent<MeshFilter>();
                meshFilter->SetMesh(meshAsset->GetMesh());
                
                // 验证MeshFilter正确设置了网格
                if (meshFilter->HasMesh()) {
                    Logger::Log("MeshFilter successfully integrated with MeshAsset");
                    auto mesh = meshFilter->GetMesh();
                    Logger::Log("  - Mesh vertex count: {}", mesh->GetVertices().size() / 8); // 假设每个顶点8个分量
                } else {
                    Logger::Warn("MeshFilter failed to integrate with MeshAsset");
                }
                
                // 添加MeshRenderer进行渲染
                auto meshRenderer = testObj->AddComponent<MeshRenderer>();
                
                // 创建材质和着色器
                auto shader = new Shader("Shaders/MeshTest.vs", "Shaders/MeshTest.fs");
                auto texture = Texture::LoadTexture("Textures/container.png");
                auto material = new Material();
                material->SetShader(shader);
                material->SetTexture("texture1", 0);
                
                meshRenderer->SetMaterial(material);
                meshRenderer->SetVisible(true);
                
                // 设置对象位置
                testObj->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
                
                AddGameObject(testObj);
                m_testObjects.push_back(testObj);
                
                Logger::Log("Created GameObject with MeshFilter and MeshRenderer");
            }
        }
        
        Logger::Log("=== MeshFilter Integration Complete ===");
    }

    void AssetManagerTestScene::TestMeshAssetMemoryManagement()
    {
        Logger::Log("=== Testing MeshAsset Memory Management ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 创建多个对同一网格资产的引用
        std::vector<std::shared_ptr<Asset>> assetRefs;
        
        auto asset = assetManager.GetAsset("TestCube");
        if (asset) {
            // 创建多个引用
            for (int i = 0; i < 5; ++i) {
                assetRefs.push_back(asset);
            }
            
            Logger::Log("Created {} references to TestCube asset", assetRefs.size());
            Logger::Log("Asset reference count: {}", asset.use_count());
            
            // 清除部分引用
            assetRefs.resize(2);
            Logger::Log("After reducing references, count: {}", asset.use_count());
            
            // 清除所有引用
            assetRefs.clear();
            Logger::Log("After clearing all local references, count: {}", asset.use_count());
        }
        
        Logger::Log("=== MeshAsset Memory Management Complete ===");
    }

    void AssetManagerTestScene::TestMeshAssetSerialization()
    {
        Logger::Log("=== Testing MeshAsset Serialization ===");
        
        // 注意：这里只是框架，实际的序列化实现可能需要额外的工作
        Logger::Log("MeshAsset serialization test - framework only");
        Logger::Log("Actual serialization implementation would go here");
        
        Logger::Log("=== MeshAsset Serialization Complete ===");
    }

    void AssetManagerTestScene::CreateMeshObjectsFromAssets()
    {
        Logger::Log("=== Creating Mesh Objects from Assets ===");
        
        auto& assetManager = AssetManager::GetInstance();
        
        // 为每个加载的网格资产创建游戏对象
        for (size_t i = 0; i < m_testMeshPaths.size(); ++i) {
            const auto& meshPath = m_testMeshPaths[i];
            auto asset = assetManager.GetAsset(meshPath);
            
            if (asset) {
                auto meshAsset = std::dynamic_pointer_cast<MeshAsset>(asset);
                if (meshAsset && meshAsset->GetMesh()) {
                    GameObject* meshObj = new GameObject();
                    meshObj->SetName("MeshObject_" + std::to_string(i));
                    
                    // 设置位置，避免重叠
                    meshObj->GetTransform()->SetPosition(glm::vec3(i * 2.0f - 2.0f, 0.0f, 0.0f));
                    
                    // 添加MeshFilter和MeshRenderer
                    auto meshFilter = meshObj->AddComponent<MeshFilter>();
                    meshFilter->SetMesh(meshAsset->GetMesh());
                    
                    auto meshRenderer = meshObj->AddComponent<MeshRenderer>();
                    
                    AddGameObject(meshObj);
                    m_testObjects.push_back(meshObj);
                    
                    Logger::Log("Created mesh object for asset: {}", meshPath);
                }
            }
        }
        
        Logger::Log("=== Mesh Objects Creation Complete ===");
    }

    void AssetManagerTestScene::TestMeshAssetStatistics()
    {
        auto& assetManager = AssetManager::GetInstance();
        
        Logger::Log("=== Mesh Asset Statistics ===");
        Logger::Log("Total loaded assets: {}", assetManager.GetTotalAssetsCount());
        
        size_t meshAssetCount = 0;
        long totalVertices = 0;
        long totalTriangles = 0;
        long totalMemory = 0;
        
        // 统计所有MeshAsset
        for (const auto& meshPath : m_testMeshPaths) {
            auto asset = assetManager.GetAsset(meshPath);
            if (asset) {
                auto meshAsset = std::dynamic_pointer_cast<MeshAsset>(asset);
                if (meshAsset) {
                    meshAssetCount++;
                    totalVertices += meshAsset->GetVertexCount();
                    totalTriangles += meshAsset->GetTriangleCount();
                    totalMemory += meshAsset->GetSize();
                }
            }
        }
        
        Logger::Log("Mesh Assets: {}", meshAssetCount);
        Logger::Log("Total Vertices: {}", totalVertices);
        Logger::Log("Total Triangles: {}", totalTriangles);
        Logger::Log("Total Memory: {} bytes", totalMemory);
        Logger::Log("Test Objects: {}", m_testObjects.size());
    }

    // void AssetManagerTestScene::PrintMeshAssetDetails(std::shared_ptr<MeshAsset> meshAsset)
    // {
    //     if (!meshAsset) {
    //         Logger::Warning("Cannot print details for null MeshAsset");
    //         return;
    //     }
        
    //     Logger::Log("MeshAsset Details:");
    //     Logger::Log("  Name: {}", meshAsset->GetName());
    //     Logger::Log("  Vertex Count: {}", meshAsset->GetVertexCount());
    //     Logger::Log("  Triangle Count: {}", meshAsset->GetTriangleCount());
    //     Logger::Log("  Size: {} bytes", meshAsset->GetSize());
    //     Logger::Log("  Reference Count: {}", meshAsset.use_count());
        
    //     auto mesh = meshAsset->GetMesh();
    //     if (mesh) {
    //         Logger::Log("  Mesh Vertices: {}", mesh->GetVertices().size());
    //         Logger::Log("  Mesh Indices: {}", mesh->GetIndices().size());
    //     } else {
    //         Logger::Log("  No mesh data available");
    //     }
    // }

    void AssetManagerTestScene::HandleTestInput()
    {
        if (Input::GetKeyDown(GLFW_KEY_1)) {
            TestMeshLoaderRegistration();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_2)) {
            TestObjMeshLoading();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_3)) {
            TestMeshAssetCreationAndManagement();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_4)) {
            TestMeshFilterIntegration();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_5)) {
            TestMeshAssetMemoryManagement();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_6)) {
            TestMeshAssetStatistics();
        }
        
        if (Input::GetKeyDown(GLFW_KEY_U)) {
            Logger::Log("Unloading all mesh assets...");
            auto& assetManager = AssetManager::GetInstance();
            for (const auto& meshPath : m_testMeshPaths) {
                assetManager.UnloadAsset(meshPath);
            }
            assetManager.UnloadAsset("TestCube");
            Logger::Log("All mesh assets unloaded");
        }
        
        if (Input::GetKeyDown(GLFW_KEY_R)) {
            Logger::Log("Reloading all mesh assets...");
            TestObjMeshLoading();
            TestMeshAssetCreationAndManagement();
            Logger::Log("All mesh assets reloaded");
        }
    }

} // namespace framework
