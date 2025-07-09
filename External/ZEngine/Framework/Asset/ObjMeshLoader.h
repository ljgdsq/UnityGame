#pragma once
#include "Framework/Asset/Asset.h"
#include "Framework/Asset/AssetLoader.h"
#include "glm/glm.hpp"
#include <string>
#include <memory>

namespace framework
{
    struct ObjVertex
    {
        glm::vec3 position;  // 顶点位置
        glm::vec3 normal;    // 法线向量
        glm::vec2 texCoords; // 纹理坐标
    };

    class ObjMeshLoader : public AssetLoader
    {
    public:
        ObjMeshLoader() = default;
        ~ObjMeshLoader() override = default;
        std::shared_ptr<Asset> LoadAsset(const std::string &assetPath) override;
        bool CanLoadAsset(const std::string &assetPath) const override;
        std::vector<std::string> GetSupportedExtensions() const override;
        std::string GetName() const override;
        AssetType GetAssetType() const override { return AssetType::Mesh; }

    private:
        bool ParseObjFile(const std::string &filePath, std::vector<ObjVertex> &vertices, std::vector<unsigned int> &indices);
        std::vector<std::string> SplitString(const std::string &str, char delimiter);
        bool ParseVertex(const std::string &line, glm::vec3 &position);
        bool ParseNormal(const std::string &line, glm::vec3 &normal);
        bool ParseTexCoord(const std::string &line, glm::vec2 &texCoord);
        bool ParseFace(const std::string &line,
                         const std::vector<glm::vec3> &positions,
                         const std::vector<glm::vec2> &texCoords,
                         const std::vector<glm::vec3> &normals,
                         std::vector<ObjVertex> &vertices,
                         std::vector<unsigned int> &indices,
                         std::unordered_map<std::string, unsigned int> &vertexCache);
        void GenerateNormals(std::vector<ObjVertex> &vertices, const std::vector<unsigned int> &indices);
    };

}