#include "Framework/Asset/ObjMeshLoader.h"
#include "Framework/Log/Logger.h"
#include "Framework/Asset/MeshAsset.h"
#include "Framework/Asset/AssetLoader.h"
#include "Framework/Graphic/Mesh.h"
#include "Framework/Core/ResLoader.h"
#include "Framework/Util/FileUtil.hpp"
#include <sstream>
#include <unordered_map>
#include "ObjMeshLoader.h"

namespace framework
{
    std::shared_ptr<Asset> ObjMeshLoader::LoadAsset(const std::string &assetPath)
    {
        Logger::Debug("Loading OBJ mesh from path: {}", assetPath);
        std::string assetName = FileUtil::ExtractFileName(assetPath);
        auto meshAsset = std::make_shared<MeshAsset>(assetName);
        std::vector<ObjVertex> vertices;
        std::vector<unsigned int> indices;
        if (!ParseObjFile(assetPath, vertices, indices))
        {
            Logger::Error("Failed to parse OBJ file: {}", assetPath);
            return nullptr;
        }

        if (vertices.empty() || indices.empty())
        {
            Logger::Error("No valid vertices or indices found in OBJ file: {}", assetPath);
            return nullptr;
        }

        // 转换为 Mesh 格式
        std::vector<float> meshVertices;
        meshVertices.reserve(vertices.size() * 8); // 3 for position, 3 for normal, 2 for texCoords
        for (const auto &vertex : vertices)
        {
            meshVertices.push_back(vertex.position.x);
            meshVertices.push_back(vertex.position.y);
            meshVertices.push_back(vertex.position.z);
            meshVertices.push_back(vertex.normal.x);
            meshVertices.push_back(vertex.normal.y);
            meshVertices.push_back(vertex.normal.z);
            meshVertices.push_back(vertex.texCoords.x);
            meshVertices.push_back(vertex.texCoords.y);
        }

        auto mesh = std::make_shared<Mesh>();
        mesh->SetVertices(meshVertices);
        mesh->SetIndices(indices);

        meshAsset->SetMesh(mesh);
        Logger::Debug("Successfully loaded OBJ mesh: {}", assetName);
        Logger::Debug("  - Vertices: {} (raw count: {})", vertices.size(), meshVertices.size() / 8);
        Logger::Debug("  - Indices: {}", indices.size());
        Logger::Debug("  - Triangles: {}", indices.size() / 3);
        return meshAsset;
    }

    bool ObjMeshLoader::CanLoadAsset(const std::string &assetPath) const
    {
        return FileUtil::HasExtension(assetPath, ".obj");
    }

    std::vector<std::string> ObjMeshLoader::GetSupportedExtensions() const
    {
        return {".obj"};
    }

    std::string ObjMeshLoader::GetName() const
    {
        return "ObjMeshLoader";
    }

    bool ObjMeshLoader::ParseObjFile(const std::string &filePath, std::vector<ObjVertex> &vertices, std::vector<unsigned int> &indices)
    {
        ResLoader &resLoader = ResLoader::GetInstance();
        std::string content = resLoader.LoadText(filePath);
        if (content.empty())
        {
            Logger::Error("Failed to load OBJ file content: {}", filePath);
            return false;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;

        // 用于顶点去重的映射
        std::unordered_map<std::string, unsigned int> vertexCache;

        std::string line;
        int lineNumber = 0;
        std::istringstream stream(content);
        while (std::getline(stream, line))
        {
            lineNumber++;

            // 跳过空行和注释
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            // 解析不同类型的行
            if (line.substr(0, 2) == "v ")
            {
                // 顶点位置
                glm::vec3 position;
                if (ParseVertex(line, position))
                {
                    positions.push_back(position);
                }
            }
            else if (line.substr(0, 3) == "vt ")
            {
                // 纹理坐标
                glm::vec2 texCoord;
                if (ParseTexCoord(line, texCoord))
                {
                    texCoords.push_back(texCoord);
                }
            }
            else if (line.substr(0, 3) == "vn ")
            {
                // 法线
                glm::vec3 normal;
                if (ParseNormal(line, normal))
                {
                    normals.push_back(normal);
                }
            }
            else if (line.substr(0, 2) == "f ")
            {
                // 面
                if (!ParseFace(line, positions, texCoords, normals, vertices, indices, vertexCache))
                {
                    Logger::Error("Failed to parse face at line {}: {}", lineNumber, line);
                    return false;
                }
            }
        }
        // 如果没有法线数据，生成默认法线
        if (normals.empty() && !vertices.empty())
        {
            Logger::Debug("No normals found, generating normals for mesh");
            GenerateNormals(vertices, indices);
        }

        Logger::Debug("OBJ parsing complete:");
        Logger::Debug("  - Input positions: {}", positions.size());
        Logger::Debug("  - Input texCoords: {}", texCoords.size());
        Logger::Debug("  - Input normals: {}", normals.size());
        Logger::Debug("  - Output vertices: {}", vertices.size());
        Logger::Debug("  - Output indices: {}", indices.size());

        return !vertices.empty() && !indices.empty();
    }
    std::vector<std::string> ObjMeshLoader::SplitString(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter))
        {
            // 去除空格
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }

        return tokens;
    }
    bool ObjMeshLoader::ParseVertex(const std::string &line, glm::vec3 &position)
    {
        std::stringstream ss(line.substr(2)); // 跳过 "v "

        if (!(ss >> position.x >> position.y >> position.z))
        {
            return false;
        }

        return true;
    }

    bool ObjMeshLoader::ParseTexCoord(const std::string &line, glm::vec2 &texCoord)
    {
        std::stringstream ss(line.substr(3)); // 跳过 "vt "

        if (!(ss >> texCoord.x >> texCoord.y))
        {
            return false;
        }

        // OBJ 纹理坐标可能需要翻转 Y 轴
        texCoord.y = 1.0f - texCoord.y;

        return true;
    }

    bool ObjMeshLoader::ParseNormal(const std::string &line, glm::vec3 &normal)
    {
        std::stringstream ss(line.substr(3)); // 跳过 "vn "

        if (!(ss >> normal.x >> normal.y >> normal.z))
        {
            return false;
        }

        return true;
    }

    bool ObjMeshLoader::ParseFace(const std::string &line,
                                  const std::vector<glm::vec3> &positions,
                                  const std::vector<glm::vec2> &texCoords,
                                  const std::vector<glm::vec3> &normals,
                                  std::vector<ObjVertex> &vertices,
                                  std::vector<unsigned int> &indices,
                                  std::unordered_map<std::string, unsigned int> &vertexCache)
    {

        std::stringstream ss(line.substr(2)); // 跳过 "f "
        std::string vertexStr;
        std::vector<unsigned int> faceIndices;

        while (ss >> vertexStr)
        {
            // 检查缓存
            auto it = vertexCache.find(vertexStr);
            if (it != vertexCache.end())
            {
                faceIndices.push_back(it->second);
                continue;
            }

            // 解析顶点字符串 (position/texCoord/normal)
            auto parts = SplitString(vertexStr, '/');
            if (parts.empty())
            {
                return false;
            }

            ObjVertex vertex;

            // 解析位置索引
            try
            {
                int posIndex = std::stoi(parts[0]) - 1; // OBJ 索引从 1 开始
                if (posIndex < 0 || posIndex >= static_cast<int>(positions.size()))
                {
                    return false;
                }
                vertex.position = positions[posIndex];
            }
            catch (const std::exception &e)
            {
                return false;
            }

            // 解析纹理坐标索引
            if (parts.size() > 1 && !parts[1].empty())
            {
                try
                {
                    int texIndex = std::stoi(parts[1]) - 1;
                    if (texIndex >= 0 && texIndex < static_cast<int>(texCoords.size()))
                    {
                        vertex.texCoords = texCoords[texIndex];
                    }
                }
                catch (const std::exception &e)
                {
                    Logger::Error("Failed to parse texture coordinate index: {}", e.what());
                }
            }

            // 解析法线索引
            if (parts.size() > 2 && !parts[2].empty())
            {
                try
                {
                    int normalIndex = std::stoi(parts[2]) - 1;
                    if (normalIndex >= 0 && normalIndex < static_cast<int>(normals.size()))
                    {
                        vertex.normal = normals[normalIndex];
                    }
                }
                catch (const std::exception &e)
                {
                    // 忽略法线解析错误
                    Logger::Error("Failed to parse normal index: {}", e.what());
                }
            }

            // 添加顶点并缓存
            unsigned int vertexIndex = static_cast<unsigned int>(vertices.size());
            vertices.push_back(vertex);
            vertexCache[vertexStr] = vertexIndex;
            faceIndices.push_back(vertexIndex);
        }

        // 三角化面（如果是四边形或更多边）
        if (faceIndices.size() >= 3)
        {
            for (size_t i = 1; i < faceIndices.size() - 1; ++i)
            {
                unsigned int i0 = faceIndices[0];
                unsigned int i1 = faceIndices[i];
                unsigned int i2 = faceIndices[i + 1];

                // 验证索引有效性
                if (i0 < vertices.size() && i1 < vertices.size() && i2 < vertices.size())
                {
                    indices.push_back(i0);
                    indices.push_back(i1);
                    indices.push_back(i2);
                }
                else
                {
                    Logger::Error("Invalid face indices: {}, {}, {} (vertex count: {})", i0, i1, i2, vertices.size());
                    return false;
                }
            }
        }

        return true;
    }

    void ObjMeshLoader::GenerateNormals(std::vector<ObjVertex> &vertices,
                                        const std::vector<unsigned int> &indices)
    {
        // 初始化所有法线为零
        for (auto &vertex : vertices)
        {
            vertex.normal = glm::vec3(0.0f);
        }

        // 计算每个三角形的法线并累加到顶点
        for (size_t i = 0; i < indices.size(); i += 3)
        {
            if (i + 2 >= indices.size())
                break;

            unsigned int i0 = indices[i];
            unsigned int i1 = indices[i + 1];
            unsigned int i2 = indices[i + 2];

            if (i0 >= vertices.size() || i1 >= vertices.size() || i2 >= vertices.size())
            {
                continue;
            }

            glm::vec3 v0 = vertices[i0].position;
            glm::vec3 v1 = vertices[i1].position;
            glm::vec3 v2 = vertices[i2].position;

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::cross(edge1, edge2);

            // 归一化法线
            if (glm::length(normal) > 0.0f)
            {
                normal = glm::normalize(normal);
            }

            vertices[i0].normal += normal;
            vertices[i1].normal += normal;
            vertices[i2].normal += normal;
        }

        // 归一化所有法线
        for (auto &vertex : vertices)
        {
            if (glm::length(vertex.normal) > 0.0f)
            {
                vertex.normal = glm::normalize(vertex.normal);
            }
        }
    }
} // namespace framework
