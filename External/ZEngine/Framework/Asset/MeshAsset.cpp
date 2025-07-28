#include "Framework/Asset/MeshAsset.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Asset/ShaderManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace framework
{
    MeshAsset::MeshAsset(const std::string &name)
        : Asset(name, AssetType::Mesh)
    {
        Logger::Debug("Created MeshAsset: {}", name);
    }

    MeshAsset::~MeshAsset()
    {
        ReleaseThumbnail();
        Logger::Debug("Destroyed MeshAsset: {}", GetName());
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

        FrameBuffer *thumbnailBuffer = new FrameBuffer(128, 128);
        thumbnailBuffer->BindBuffer();
        // save old viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        glViewport(0, 0, 128, 128);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 使用缩略图着色器
        auto shader = ShaderManager::GetInstance().GetShader("Shaders/Thumbnail")->GetShader();
        shader->Use();

        // 设置变换矩阵
        // 创建绕X轴旋转45度的模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕X轴旋转45度
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 绕Y轴旋转30度以获得更好的视角

        // 设置视图矩阵（相机位置）
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f), // 相机位置
            glm::vec3(0.0f, 0.0f, 0.0f), // 看向原点
            glm::vec3(0.0f, 1.0f, 0.0f)  // 上方向
        );

        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        // 将矩阵传递给着色器
        shader->SetMatrix4("model", model);
        shader->SetMatrix4("view", view);
        shader->SetMatrix4("projection", projection);

        // 绘制网格
        m_mesh->Use();
        glDrawElements(GL_TRIANGLES, m_mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
        thumbnailBuffer->UnbindBuffer();
        m_thumbnailTextureId = reinterpret_cast<void *>(thumbnailBuffer->GetColorBuffer());
        // 恢复旧的viewport
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        Logger::Debug("Generated thumbnail for mesh: {}", GetName());
    }

} // namespace framework
