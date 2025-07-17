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
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/MeshAsset.h"
#include <vector>

namespace framework
{
    class AssetManagerTestScene : public Scene
    {
    public:
        AssetManagerTestScene() = default;
        ~AssetManagerTestScene() override = default;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(Renderer *renderer) override;
        void Shutdown() override;

        const char *GetName() const override { return "AssetManagerTestScene"; }

    private:
        // 测试用的游戏对象
        std::vector<GameObject *> m_testObjects;

        // UI显示相关
        float m_uiUpdateTimer = 0.0f;
        const float m_uiUpdateInterval = 1.0f; // 每秒更新一次UI信息

        // 测试状态
        bool m_assetsLoaded = false;
        int m_currentTestIndex = 0;
        float m_testTimer = 0.0f;

        // 测试资源名称列表 - 专注于网格资源
        std::vector<std::string> m_testMeshPaths = {
            "Res/Models/primitives/Cube.obj"};

        // 测试方法 - 专注于MeshAsset, MeshLoader, MeshFilter
        void TestMeshLoaderRegistration();
        void TestObjMeshLoading();
        void TestMeshAssetCreationAndManagement();
        void TestMeshFilterIntegration();
        void TestMeshAssetMemoryManagement();
        void TestMeshAssetSerialization();
        void CreateMeshObjectsFromAssets();
        void TestMeshAssetStatistics();
        void HandleTestInput();
    };
} // namespace framework
