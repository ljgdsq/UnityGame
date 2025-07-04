#pragma once
#include "Framework/Component/Component.h"
#include "Framework/Render/Mesh.h"
#include <memory>

namespace framework {

/**
 * @brief 网格过滤器组件
 * 存储和管理网格数据
 */
class MeshFilter : public ComponentBase<MeshFilter> {
public:
    MeshFilter(GameObject* owner);
    ~MeshFilter() = default;

    // 设置网格数据（共享模式）
    void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
    
    // 设置网格数据（独占模式）
    void SetMesh(std::unique_ptr<Mesh> mesh) { m_mesh = std::move(mesh); }
    
    // 创建新的网格数据
    void CreateMesh() { m_mesh = std::make_shared<Mesh>(); }
    
    // 获取网格数据
    std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
    
    // 检查是否有有效网格
    bool HasMesh() const { return m_mesh && !m_mesh->GetVertices().empty(); }

private:
    std::shared_ptr<Mesh> m_mesh; // 存储网格数据
};

} // namespace framework
