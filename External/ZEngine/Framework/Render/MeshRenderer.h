#pragma once
#include "Framework/Render/RenderComponent.h"

namespace framework {

class MeshFilter;
class GameObject;
/**
 * @brief 网格渲染器组件
 * 负责渲染MeshFilter中的网格数据
 */
class MeshRenderer : public RenderComponent {
public:
    MeshRenderer(GameObject* owner);
    virtual ~MeshRenderer() = default;

    // RenderComponent接口实现
    void Render(Renderer* renderer) override;
    virtual void OnInitialize() override;
private:
    MeshFilter* m_meshFilter = nullptr; // 缓存MeshFilter组件引用
    
    // 获取MeshFilter组件
    MeshFilter* GetMeshFilter();
};

} // namespace framework

