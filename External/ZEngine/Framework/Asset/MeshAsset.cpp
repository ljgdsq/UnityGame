#include "Framework/Asset/MeshAsset.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/ResLoader.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace framework
{
    MeshAsset::MeshAsset(const std::string &name)
        : Asset(name, AssetType::Mesh)
    {
        Logger::Debug("Created MeshAsset: {}", name);
    }

    MeshAsset::MeshAsset(const std::string &name, const std::string &assetId)
        : Asset(name, AssetType::Mesh, assetId)
    {
        Logger::Debug("Created MeshAsset: {} (ID: {})", name, assetId);
    }

    MeshAsset::~MeshAsset()
    {
        Unload();
        Logger::Debug("Destroyed MeshAsset: {}", GetName());
    }

    void MeshAsset::Load()
    {
        if (IsLoaded())
        {
            return;
        }

        SetLoadState(LoadState::Loading);

        try
        {
            if (GetFilePath().empty())
            {
                Logger::Error("MeshAsset::Load - No file path set for asset: {}", GetName());
                SetLoadState(LoadState::Failed);
                return;
            }

            if (LoadFromFile(GetFilePath()))
            {
                SetLoadState(LoadState::Loaded);
                Logger::Debug("Successfully loaded mesh: {}", GetName());
            }
            else
            {
                SetLoadState(LoadState::Failed);
                Logger::Error("Failed to load mesh: {}", GetName());
            }
        }
        catch (const std::exception &e)
        {
            SetLoadState(LoadState::Failed);
            Logger::Error("Exception while loading mesh {}: {}", GetName(), e.what());
        }
    }

    void MeshAsset::Unload()
    {
        if (!IsLoaded())
        {
            return;
        }

        m_mesh.reset();
        m_vertexCount = 0;
        m_triangleCount = 0;

        SetLoadState(LoadState::NotLoaded);
        Logger::Debug("Unloaded mesh: {}", GetName());
    }

    long MeshAsset::GetSize() const
    {
        if (!m_mesh)
        {
            return 0;
        }

        // 估算网格内存大小
        long size = 0;

        // 顶点数据大小
        size += m_mesh->GetVertices().size() * sizeof(float);

        // 索引数据大小
        size += m_mesh->GetIndices().size() * sizeof(unsigned int);

        return size;
    }

    std::shared_ptr<Mesh> MeshAsset::GetMesh() const
    {
        return m_mesh;
    }

    void MeshAsset::SetMesh(const std::shared_ptr<Mesh> &mesh)
    {
        m_mesh = mesh;
        if (mesh)
        {
            UpdateMeshStatistics();
            GenerateThumbnail();
        }
        else
        {
            m_vertexCount = 0;
            m_triangleCount = 0;
        }
    }

    long MeshAsset::GetVertexCount() const
    {
        return m_vertexCount;
    }

    long MeshAsset::GetTriangleCount() const
    {
        return m_triangleCount;
    }

    rapidjson::Value MeshAsset::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue = Asset::Serialize(allocator);

        // 添加MeshAsset特有的属性
        jsonValue.AddMember("vertexCount", static_cast<int64_t>(m_vertexCount), allocator);
        jsonValue.AddMember("triangleCount", static_cast<int64_t>(m_triangleCount), allocator);

        return jsonValue;
    }

    void MeshAsset::Deserialize(const rapidjson::Value &json)
    {
        Asset::Deserialize(json);

        if (json.HasMember("vertexCount") && json["vertexCount"].IsInt64())
        {
            m_vertexCount = json["vertexCount"].GetInt64();
        }

        if (json.HasMember("triangleCount") && json["triangleCount"].IsInt64())
        {
            m_triangleCount = json["triangleCount"].GetInt64();
        }
    }

    bool MeshAsset::LoadFromFile(const std::string &filePath)
    {
        // 初始化ResLoader
        ResLoader::GetInstance().Initialize("Res/");

        // 解析文件路径
        std::string resolvedPath;

        // 尝试不同的路径解析方法
        if (std::filesystem::exists(filePath))
        {
            resolvedPath = filePath;
        }
        else
        {
            // 尝试通过ResLoader查找文件
            resolvedPath = ResLoader::GetInstance().GetResourcePath(filePath);
            if (resolvedPath.empty() || !std::filesystem::exists(resolvedPath))
            {
                // 尝试在Models目录查找
                std::filesystem::path pathObj(filePath);
                std::string filename = pathObj.filename().string();
                resolvedPath = ResLoader::GetInstance().GetResourcePath("Models/" + filename);
            }
        }

        if (resolvedPath.empty() || !std::filesystem::exists(resolvedPath))
        {
            Logger::Error("Could not resolve mesh path: {}", filePath);
            return false;
        }

        // 获取文件扩展名
        std::filesystem::path pathObj(resolvedPath);
        std::string extension = pathObj.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // 根据文件格式加载
        if (extension == ".obj")
        {
            return LoadOBJ(resolvedPath);
        }
        else if (extension == ".mesh")
        {
            return LoadZEngineMesh(resolvedPath);
        }
        else
        {
            Logger::Error("Unsupported mesh format: {}", extension);
            return false;
        }
    }

    bool MeshAsset::LoadOBJ(const std::string &filePath)
    {
        // 简化的OBJ加载器实现
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            Logger::Error("Failed to open OBJ file: {}", filePath);
            return false;
        }

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
            else if (prefix == "vn")
            {
                float x, y, z;
                iss >> x >> y >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            else if (prefix == "vt")
            {
                float u, v;
                iss >> u >> v;
                texCoords.push_back(u);
                texCoords.push_back(v);
            }
            else if (prefix == "f")
            {
                // 简化处理：只处理三角面，且假设格式为 v/vt/vn
                std::string vertex1, vertex2, vertex3;
                iss >> vertex1 >> vertex2 >> vertex3;

                auto parseVertex = [](const std::string &str) -> std::tuple<int, int, int>
                {
                    std::istringstream vss(str);
                    std::string token;
                    std::vector<int> values;

                    while (std::getline(vss, token, '/'))
                    {
                        if (!token.empty())
                            values.push_back(std::stoi(token) - 1); // OBJ索引从1开始
                        else
                            values.push_back(-1);
                    }

                    while (values.size() < 3)
                        values.push_back(-1);

                    return {values[0], values[1], values[2]};
                };

                auto [v1, vt1, vn1] = parseVertex(vertex1);
                auto [v2, vt2, vn2] = parseVertex(vertex2);
                auto [v3, vt3, vn3] = parseVertex(vertex3);

                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);
            }
        }

        // 创建Mesh对象
        m_mesh = std::make_shared<Mesh>();

        // 组装顶点数据
        std::vector<float> meshData;
        for (size_t i = 0; i < indices.size(); ++i)
        {
            unsigned int vertexIndex = indices[i];

            // 位置
            if (vertexIndex < vertices.size() / 3)
            {
                meshData.push_back(vertices[vertexIndex * 3]);
                meshData.push_back(vertices[vertexIndex * 3 + 1]);
                meshData.push_back(vertices[vertexIndex * 3 + 2]);
            }
            else
            {
                meshData.push_back(0.0f);
                meshData.push_back(0.0f);
                meshData.push_back(0.0f);
            }

            // 法线（如果有）
            if (vertexIndex < normals.size() / 3)
            {
                meshData.push_back(normals[vertexIndex * 3]);
                meshData.push_back(normals[vertexIndex * 3 + 1]);
                meshData.push_back(normals[vertexIndex * 3 + 2]);
            }
            else
            {
                meshData.push_back(0.0f);
                meshData.push_back(1.0f);
                meshData.push_back(0.0f);
            }

            // UV坐标（如果有）
            if (vertexIndex < texCoords.size() / 2)
            {
                meshData.push_back(texCoords[vertexIndex * 2]);
                meshData.push_back(texCoords[vertexIndex * 2 + 1]);
            }
            else
            {
                meshData.push_back(0.0f);
                meshData.push_back(0.0f);
            }
        }

        // 设置顶点数据
        m_mesh->SetVertices(meshData);

        // 更新统计信息
        UpdateMeshStatistics();

        Logger::Debug("Loaded OBJ mesh: {} vertices, {} triangles", m_vertexCount, m_triangleCount);
        return true;
    }

    bool MeshAsset::LoadZEngineMesh(const std::string &filePath)
    {
        // ZEngine自定义网格格式加载器
        Logger::Warn("ZEngine mesh format not implemented yet: {}", filePath);
        return false;
    }

    void MeshAsset::UpdateMeshStatistics()
    {
        if (!m_mesh)
        {
            m_vertexCount = 0;
            m_triangleCount = 0;
            return;
        }

        // 计算顶点数量（假设每个顶点有8个浮点数：3位置+3法线+2UV）
        m_vertexCount = m_mesh->GetVertices().size() / 8;

        // 计算三角形数量
        if (m_mesh->GetIndices().empty())
        {
            m_triangleCount = m_vertexCount / 3;
        }
        else
        {
            m_triangleCount = m_mesh->GetIndices().size() / 3;
        }
    }

    void MeshAsset::GenerateThumbnail()
    {
        if (!m_mesh)
        {
            return;
        }

        // 生成网格缩略图
        const int thumbWidth = 64;
        const int thumbHeight = 64;
        const int channels = 3;

        std::vector<unsigned char> thumbnailData(thumbWidth * thumbHeight * channels);

        // 生成简单的渐变图作为占位
        for (int y = 0; y < thumbHeight; ++y)
        {
            for (int x = 0; x < thumbWidth; ++x)
            {
                int index = (y * thumbWidth + x) * channels;
                thumbnailData[index] = static_cast<unsigned char>(x * 255 / thumbWidth);      // R
                thumbnailData[index + 1] = static_cast<unsigned char>(y * 255 / thumbHeight); // G
                thumbnailData[index + 2] = 128;                                               // B
            }
        }

        SetThumbnailTextureId(nullptr); // 简化处理，暂时不设置纹理ID
        Logger::Debug("Generated thumbnail for mesh: {}", GetName());
    }

    void MeshAsset::CreatePrimitive(MeshPrimitive primitive)
    {
        m_mesh = std::make_shared<Mesh>();

        switch (primitive)
        {
        case MeshPrimitive::Cube:
            CreateCube();
            break;
        case MeshPrimitive::Sphere:
            CreateSphere();
            break;
        case MeshPrimitive::Plane:
            CreatePlane();
            break;
        default:
            Logger::Error("Unsupported mesh primitive: {}", static_cast<int>(primitive));
            return;
        }

        UpdateMeshStatistics();
        GenerateThumbnail();
    }

    void MeshAsset::CreateCube()
    {
        // 立方体顶点数据 (位置 + 法线 + UV)
        std::vector<float> cubeVertices = {
            // 前面
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            // 后面
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            1.0f,
            // 左面
            -0.5f,
            -0.5f,
            -0.5f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            // 右面
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            // 上面
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            // 下面
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
        };

        // 立方体索引
        std::vector<unsigned int> cubeIndices = {
            0, 1, 2, 2, 3, 0,       // 前面
            4, 5, 6, 6, 7, 4,       // 后面
            8, 9, 10, 10, 11, 8,    // 左面
            12, 13, 14, 14, 15, 12, // 右面
            16, 17, 18, 18, 19, 16, // 上面
            20, 21, 22, 22, 23, 20  // 下面
        };

        m_mesh->SetVertices(cubeVertices);
        m_mesh->SetIndices(cubeIndices);

        Logger::Debug("Created cube mesh for: {}", GetName());
    }

    void MeshAsset::CreateSphere()
    {
        // 简化的球体生成
        const int segments = 16;
        const int rings = 16;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // 生成球体顶点
        for (int ring = 0; ring <= rings; ++ring)
        {
            float theta = static_cast<float>(ring) * M_PI / rings;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int segment = 0; segment <= segments; ++segment)
            {
                float phi = static_cast<float>(segment) * 2.0f * M_PI / segments;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                // 位置
                vertices.push_back(x * 0.5f);
                vertices.push_back(y * 0.5f);
                vertices.push_back(z * 0.5f);

                // 法线
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // UV
                vertices.push_back(static_cast<float>(segment) / segments);
                vertices.push_back(static_cast<float>(ring) / rings);
            }
        }

        // 生成球体索引
        for (int ring = 0; ring < rings; ++ring)
        {
            for (int segment = 0; segment < segments; ++segment)
            {
                int current = ring * (segments + 1) + segment;
                int next = current + segments + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        m_mesh->SetVertices(vertices);
        m_mesh->SetIndices(indices);

        Logger::Debug("Created sphere mesh for: {}", GetName());
    }

    void MeshAsset::CreatePlane()
    {
        // 平面顶点数据
        std::vector<float> planeVertices = {
            // 位置           法线           UV
            -0.5f,
            0.0f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            0.0f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            0.0f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.0f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
        };

        // 平面索引
        std::vector<unsigned int> planeIndices = {
            0, 1, 2,
            2, 3, 0};

        m_mesh->SetVertices(planeVertices);
        m_mesh->SetIndices(planeIndices);

        Logger::Debug("Created plane mesh for: {}", GetName());
    }

} // namespace framework
