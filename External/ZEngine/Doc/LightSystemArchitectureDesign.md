# Light System Architecture Design - 光照系统架构设计

## 概述

本文档详细描述了ZEngine中Light组件数据传递给Shader的架构设计。该设计旨在建立一个高效、可扩展的光照系统，支持多种光源类型和多光源渲染。

## 当前架构分析

### 现有渲染流程
```
Scene → GameObject(MeshRenderer) → Material → Shader
  ↓
Transform → 模型矩阵
Camera → 视图/投影矩阵
```

### 当前组件关系
- **MeshRenderer**: 负责渲染单个网格对象
- **Material**: 管理着色器参数和纹理
- **Transform**: 提供对象变换信息
- **CameraManager**: 管理相机和提供视图矩阵
- **Light**: 光源组件（已存在但未集成到渲染管线）

### 当前限制
1. Light组件数据无法传递给Shader
2. 缺乏统一的光照管理机制
3. 不支持多光源渲染
4. 光照计算分散，难以优化

## 设计目标

1. **统一管理**: 集中管理场景中的所有光源
2. **高性能**: 支持高效的多光源渲染
3. **可扩展**: 支持不同类型的光源（方向光、点光源、聚光灯等）
4. **易用性**: 简化光照的使用和配置
5. **向后兼容**: 与现有渲染架构兼容

## 推荐架构：集中式光照管理

### 整体架构流程
```
Scene → LightManager → Material/Shader
  ↓
GameObject(Light) → LightManager.CollectLights()
  ↓
MeshRenderer.Render() → LightManager.ApplyLights(material)
  ↓
Shader接收标准化光照数据
```

### 核心组件设计

#### 1. LightManager (单例模式)

**职责:**
- 收集和管理场景中的所有Light组件
- 维护光源数据的缓存和更新
- 提供标准化的光照数据接口
- 执行光照优化（剔除、排序、限制）

**主要接口:**
```cpp
class LightManager {
public:
    static LightManager& GetInstance();
    
    // 光源注册和管理
    void RegisterLight(Light* light);
    void UnregisterLight(Light* light);
    void UpdateLightData();
    
    // 渲染时光照应用
    void ApplyLights(Material* material);
    void ApplyLights(Material* material, const glm::vec3& objectPosition);
    
    // 光源查询
    const std::vector<LightData>& GetActiveLights() const;
    std::vector<LightData> GetNearbyLights(const glm::vec3& position, float radius) const;
    
    // 配置
    void SetMaxLights(int maxLights);
    void EnableLightCulling(bool enable);
    
private:
    std::vector<Light*> m_registeredLights;
    std::vector<LightData> m_lightDataCache;
    bool m_needsUpdate = true;
    int m_maxActiveLights = 8;
    bool m_enableCulling = true;
};
```

#### 2. LightData 标准化结构

**设计原则:**
- 统一不同类型光源的数据格式
- 优化内存布局以提高性能
- 直接映射到Shader uniform结构

```cpp
struct LightData {
    // 光源类型 (0=方向光, 1=点光源, 2=聚光灯, 3=区域光)
    int type;
    
    // 位置信息
    glm::vec3 position;    // 点光源/聚光灯的世界位置
    glm::vec3 direction;   // 方向光/聚光灯的方向向量
    
    // 光照属性
    glm::vec3 color;       // 光源颜色 (RGB)
    float intensity;       // 光强
    
    // 衰减参数
    float range;           // 影响范围 (点光源/聚光灯)
    float constant;        // 常数衰减
    float linear;          // 线性衰减
    float quadratic;       // 二次衰减
    
    // 聚光灯特有参数
    float spotInnerAngle;  // 内锥角 (cos值)
    float spotOuterAngle;  // 外锥角 (cos值)
    
    // 阴影相关 (未来扩展)
    int castShadows;       // 是否投射阴影
    float shadowBias;      // 阴影偏移
    
    // 构造函数
    LightData();
    static LightData FromLight(const Light* light);
};
```

#### 3. Material 光照接口扩展

**新增光照相关方法:**
```cpp
class Material {
public:
    // 现有方法...
    
    // 光照相关uniform设置
    void SetLightCount(int count);
    void SetLightData(const std::vector<LightData>& lights);
    void SetAmbientLight(const glm::vec3& ambient);
    
    // 材质光照属性
    void SetDiffuseColor(const glm::vec3& color);
    void SetSpecularColor(const glm::vec3& color);
    void SetShininess(float shininess);
    void SetMetallic(float metallic);
    void SetRoughness(float roughness);
    
private:
    void UpdateLightUniforms(const std::vector<LightData>& lights);
};
```

#### 4. Light 组件接口规范

**确保Light组件提供标准接口:**
```cpp
class Light : public Component {
public:
    // 基础属性访问
    LightType GetLightType() const;
    glm::vec3 GetColor() const;
    float GetIntensity() const;
    float GetRange() const;
    
    // 位置和方向 (通过Transform获取)
    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetWorldDirection() const;
    
    // 衰减参数
    float GetConstantAttenuation() const;
    float GetLinearAttenuation() const;
    float GetQuadraticAttenuation() const;
    
    // 聚光灯参数
    float GetSpotInnerAngle() const;
    float GetSpotOuterAngle() const;
    
    // 状态管理
    bool IsEnabled() const;
    void SetEnabled(bool enabled);
    
    // 生命周期回调
    void OnEnable() override;
    void OnDisable() override;
};
```

## 数据流设计

### 1. 初始化阶段

```
Scene加载
  ↓
Light组件创建 → Light::OnEnable() → LightManager::RegisterLight()
  ↓
LightManager建立光源列表
```

### 2. 每帧更新阶段

```
Scene::Update()
  ↓
Light组件更新 → 标记LightManager需要更新
  ↓
LightManager::UpdateLightData() → 重建光照数据缓存
```

### 3. 渲染阶段

```
MeshRenderer::Render()
  ↓
LightManager::ApplyLights(material, objectPosition)
  ↓
Material::SetLightData() → 设置Shader uniform
  ↓
Shader接收光照数据进行计算
```

## Shader接口设计

### Uniform结构定义

```glsl
// 最大支持光源数量
#define MAX_LIGHTS 8

// 光源数据结构
struct LightData {
    int type;              // 光源类型
    vec3 position;         // 位置
    vec3 direction;        // 方向
    vec3 color;            // 颜色
    float intensity;       // 强度
    float range;           // 范围
    float constant;        // 常数衰减
    float linear;          // 线性衰减
    float quadratic;       // 二次衰减
    float spotInnerAngle;  // 聚光灯内角
    float spotOuterAngle;  // 聚光灯外角
};

// Uniform变量
uniform int u_lightCount;
uniform LightData u_lights[MAX_LIGHTS];
uniform vec3 u_ambientLight;
uniform vec3 u_viewPosition;

// 材质属性
uniform vec3 u_diffuseColor;
uniform vec3 u_specularColor;
uniform float u_shininess;
uniform float u_metallic;
uniform float u_roughness;
```

### 光照计算函数

```glsl
// 计算单个光源的贡献
vec3 CalculateLight(LightData light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

// 计算方向光
vec3 CalculateDirectionalLight(LightData light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

// 计算点光源
vec3 CalculatePointLight(LightData light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

// 计算聚光灯
vec3 CalculateSpotLight(LightData light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

// 主光照计算
vec3 CalculateLighting(vec3 normal, vec3 fragPos, vec3 diffuseColor, vec3 specularColor) {
    vec3 result = u_ambientLight * diffuseColor;
    vec3 viewDir = normalize(u_viewPosition - fragPos);
    
    for(int i = 0; i < u_lightCount; ++i) {
        result += CalculateLight(u_lights[i], normal, fragPos, viewDir, diffuseColor, specularColor);
    }
    
    return result;
}
```

## 性能优化策略

### 1. 光源剔除

**距离剔除:**
```cpp
std::vector<LightData> GetNearbyLights(const glm::vec3& position, float radius) {
    std::vector<LightData> nearbyLights;
    for(const auto& lightData : m_lightDataCache) {
        if(lightData.type == DIRECTIONAL_LIGHT) {
            nearbyLights.push_back(lightData);
        } else {
            float distance = glm::distance(position, lightData.position);
            if(distance <= lightData.range + radius) {
                nearbyLights.push_back(lightData);
            }
        }
    }
    return nearbyLights;
}
```

**视锥剔除:**
- 检查光源是否在相机视锥体内
- 对于点光源，检查球体与视锥体的相交
- 对于聚光灯，检查锥体与视锥体的相交

### 2. 光源排序和限制

**影响力排序:**
```cpp
void SortLightsByInfluence(std::vector<LightData>& lights, const glm::vec3& objectPosition) {
    std::sort(lights.begin(), lights.end(), [&](const LightData& a, const LightData& b) {
        // 方向光优先级最高
        if(a.type == DIRECTIONAL_LIGHT && b.type != DIRECTIONAL_LIGHT) return true;
        if(b.type == DIRECTIONAL_LIGHT && a.type != DIRECTIONAL_LIGHT) return false;
        
        // 按距离和强度计算影响力
        float influenceA = CalculateInfluence(a, objectPosition);
        float influenceB = CalculateInfluence(b, objectPosition);
        return influenceA > influenceB;
    });
    
    // 限制最大光源数量
    if(lights.size() > m_maxActiveLights) {
        lights.resize(m_maxActiveLights);
    }
}
```

### 3. 批量处理优化

**材质分组:**
- 相同Material的对象共享光照设置
- 减少重复的uniform设置调用

**光照数据缓存:**
- 缓存每帧的光照数据
- 只在光源变化时重新计算

## 集成策略

### 阶段1: 基础LightManager实现

**目标:**
- 创建LightManager单例
- 实现基础的Light注册和收集
- 支持单一方向光

**集成点:**
```cpp
// 在Scene::Update()中
LightManager::GetInstance().UpdateLightData();

// 在MeshRenderer::Render()中
if (m_material) {
    m_material->Use();
    LightManager::GetInstance().ApplyLights(m_material);
    // ...现有代码
}
```

### 阶段2: Material光照接口扩展

**目标:**
- 扩展Material类支持光照uniform
- 实现基础的光照Shader支持

### 阶段3: 多光源支持

**目标:**
- 支持多种光源类型
- 实现光源限制和排序
- 添加光照剔除优化

### 阶段4: 高级功能

**目标:**
- 实现高级光照模型（PBR）
- 添加阴影支持
- 性能监控和调试工具

## 向后兼容性

### 现有代码保护
- 现有的Material uniform设置方法保持不变
- MeshRenderer的基础渲染流程不变
- 新增的光照功能为可选特性

### 迁移路径
- 现有Shader可以逐步升级支持光照
- 提供默认的光照着色器模板
- 支持传统的固定管线渲染（如果需要）

## 调试和监控

### 调试工具
- Light组件的Inspector面板
- 光照数据的运行时查看器
- 性能统计（活跃光源数量、剔除统计等）

### 性能监控
```cpp
struct LightingStats {
    int totalLights;        // 场景总光源数
    int activeLights;       // 活跃光源数
    int culledLights;       // 被剔除的光源数
    float updateTime;       // 光照更新时间
    float applyTime;        // 光照应用时间
};
```

## 未来扩展

### 阴影系统
- Shadow Mapping支持
- Cascaded Shadow Maps (CSM)
- Point Light Shadows (立方体贴图)

### 高级光照
- 基于物理的渲染(PBR)
- 全局光照(GI)
- 体积光照

### 性能优化
- 光照瓦片化(Tiled Lighting)
- 光照聚类(Clustered Lighting)
- GPU光照剔除

## 结论

该设计提供了一个完整、可扩展的光照系统架构，能够：

1. **统一管理**场景中的所有光源
2. **高效传递**光照数据到Shader
3. **支持多光源**渲染
4. **保持性能**通过各种优化策略
5. **易于扩展**支持未来的高级功能

通过分阶段实现，可以在保持系统稳定性的同时，逐步构建出功能完整的光照系统。
