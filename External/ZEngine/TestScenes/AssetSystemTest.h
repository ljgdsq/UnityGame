#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureAsset.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"

namespace framework
{
    class AssetSystemTest
    {
    public:
        static void RunTests()
        {
            Logger::Log("Starting Asset System Tests...");

            TestAssetManager();
            TestTextureAsset();
            TestMeshAsset();
            TestAssetManagerAdvanced();

            Logger::Log("Asset System Tests Completed!");
        }

    private:
        static void TestAssetManager()
        {
            Logger::Log("Testing AssetManager with integrated registry features...");

            AssetManager &manager = AssetManager::GetInstance();

            // 测试资源注册
            AssetInfo textureInfo;
            textureInfo.assetId = "test_texture_001";
            textureInfo.displayName = "TestTexture";
            textureInfo.type = AssetType::Texture;
            textureInfo.filePath = "Textures/test.png";
            textureInfo.fileSize = 1024;

            manager.RegisterAssetInfo(textureInfo);

            // 测试资源查找
            auto foundAsset = manager.GetAssetInfo("test_texture_001");
            if (foundAsset)
            {
                Logger::Log("Found asset: {} ({})", foundAsset->displayName, foundAsset->assetId);
            }
            else
            {
                Logger::Error("Failed to find registered asset");
            }

            // 测试按类型查找
            auto textureAssets = manager.GetAssetInfosByType(AssetType::Texture);
            Logger::Log("Found {} texture assets", textureAssets.size());
        }

        static void TestTextureAsset()
        {
            Logger::Log("Testing TextureAsset...");

            auto textureAsset = std::make_shared<TextureAsset>("TestTexture", "test_texture_001");
            textureAsset->SetFilePath("Textures/test.png");

            // 测试序列化
            rapidjson::Document doc;
            doc.SetObject();
            auto json = textureAsset->Serialize(doc.GetAllocator());

            Logger::Log("Serialized texture asset");

            // 测试反序列化
            auto newTextureAsset = std::make_shared<TextureAsset>("NewTexture");
            newTextureAsset->Deserialize(json);

            Logger::Log("Deserialized texture asset: {}", newTextureAsset->GetName());
        }

        static void TestMeshAsset()
        {
            Logger::Log("Testing MeshAsset...");

            auto meshAsset = std::make_shared<MeshAsset>("TestMesh", "test_mesh_001");

            // 测试基本创建
            Logger::Log("Created mesh asset: {}", meshAsset->GetName());

            // 测试序列化
            rapidjson::Document doc;
            doc.SetObject();
            auto json = meshAsset->Serialize(doc.GetAllocator());

            Logger::Log("Serialized mesh asset");
        }

        static void TestAssetManagerAdvanced()
        {
            Logger::Log("Testing AssetManager advanced features...");

            AssetManager &manager = AssetManager::GetInstance();

            // 测试基本统计信息
            Logger::Log("Total assets count: {}", manager.GetTotalAssetsCount());
            Logger::Log("Total assets size: {} bytes", manager.GetTotalAssetsSize());

            auto assetNames = manager.GetAllAssetNames();
            Logger::Log("Asset names count: {}", assetNames.size());
        }
    };

} // namespace framework
