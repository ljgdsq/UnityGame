# Scene & GameObject 渲染系统设计文档

## 概述

ZEngine采用基于场景(Scene)和游戏对象(GameObject)的渲染架构，提供了灵活且高效的渲染管道。该系统遵循实体组件系统(ECS)的设计原则，通过组件化的方式来管理和渲染3D对象。

## 核心架构

### 1. 场景管理系统 (Scene Management)

#### 1.1 Scene 基类
```cpp
class Scene {
public:
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
    virtual const char* GetName() const = 0;
};
```

#### 1.2 SceneManager 单例
- **功能**: 管理场景的生命周期，处理场景切换
- **主要方法**:
  - `AddScene(name, scene)`: 添加场景到管理器
  - `SetActiveScene(name)`: 设置活动场景
  - `UpdateActiveScene(deltaTime)`: 更新活动场景
  - `RenderActiveScene()`: 渲染活动场景

#### 1.3 场景生命周期
1. **Initialize**: 场景初始化，创建游戏对象和组件
2. **Update**: 每帧更新逻辑
3. **Render**: 渲染场景中的所有对象
4. **Shutdown**: 清理资源

### 2. 游戏对象系统 (GameObject System)

#### 2.1 GameObject 核心特性
- **组件容器**: 存储和管理各种组件
- **层次结构**: 支持父子关系的层次结构
- **生命周期管理**: 控制对象的激活状态
- **默认组件**: 每个GameObject都有一个Transform组件

#### 2.2 组件管理
```cpp
template<typename T>
T* AddComponent();           // 添加组件

template<typename T>
T* GetComponent() const;     // 获取组件

template<typename T>
void RemoveComponent();      // 移除组件
```

### 3. 渲染组件系统 (Render Component System)

#### 3.1 RenderComponent 基类
```cpp
class RenderComponent : public ComponentBase<RenderComponent> {
public:
    virtual void Render(Renderer* renderer) = 0;
    
    // 可见性控制
    bool IsVisible() const;
    void SetVisible(bool visible);
    
    // 材质管理
    void SetMaterial(Material* material);
    Material* GetMaterial() const;
    
    // 渲染层级和排序
    int GetRenderLayer() const;
    void SetRenderLayer(int layer);
    int GetSortingOrder() const;
    void SetSortingOrder(int order);
};
```

#### 3.2 MeshRenderer 组件
- **功能**: 渲染网格数据
- **依赖**: 需要MeshFilter组件提供网格数据
- **渲染流程**:
  1. 检查可见性和网格数据
  2. 获取Transform组件的变换矩阵
  3. 应用材质和着色器
  4. 执行OpenGL绘制调用

#### 3.3 MeshFilter 组件
- **功能**: 存储和管理网格数据
- **支持**: 共享和独占两种网格存储模式
- **网格数据**: 包含顶点数据和索引数据

### 4. 材质和着色器系统

#### 4.1 Material 类
- **功能**: 管理着色器和纹理资源
- **属性设置**: 支持各种类型的着色器参数
- **渲染状态**: 管理OpenGL渲染状态

#### 4.2 Shader 类
- **功能**: 封装OpenGL着色器程序
- **参数设置**: 提供统一的参数设置接口
- **资源管理**: 自动管理着色器生命周期

### 5. 相机系统 (Camera System)

#### 5.1 Camera 组件
- **投影矩阵**: 支持透视和正交投影
- **视图矩阵**: 基于Transform组件计算
- **渲染参数**: 清除颜色、深度测试等

#### 5.2 CameraManager 单例
- **主相机管理**: 设置和获取主相机
- **多相机支持**: 管理多个相机实例
- **视角切换**: 支持动态切换相机视角

## 渲染流程

### 渲染流程详解

这种基于组件收集和排序的渲染流程具有以下核心优势：

1. **解耦渲染逻辑**: 将对象遍历和渲染逻辑分离，使得渲染系统更加灵活
2. **统一排序**: 可以对所有渲染组件进行全局排序，优化渲染顺序
3. **批量优化**: 相同材质的对象可以连续渲染，减少状态切换
4. **可扩展性**: 可以轻松添加新的排序策略和渲染优化

### 排序策略

渲染组件的排序通常考虑以下因素：

1. **渲染层级 (Render Layer)**
   - 用于控制渲染顺序的大分类
   - 例如：背景层(0) < 游戏对象层(1) < UI层(2)

2. **排序顺序 (Sorting Order)**
   - 同一层级内的细粒度排序
   - 数值越小越先渲染

3. **材质类型**
   - 相同材质的对象可以批量渲染
   - 减少GPU状态切换

4. **深度距离**
   - 透明对象需要从后往前渲染
   - 不透明对象可以从前往后渲染（Early Z优化）

### 1. 主渲染循环 (Main Render Loop)

```cpp
while (!ShouldExit()) {
    // 1. 输入处理
    Input::Update();
    HandleInput();
    
    // 2. 更新逻辑
    Update(deltaTime);
    SceneManager::GetInstance().UpdateActiveScene(deltaTime);
    
    // 3. 渲染
    renderer->Clear();
    SceneManager::GetInstance().RenderActiveScene();
    renderer->SwapBuffers();
}
```

### 2. 场景渲染流程

```cpp
void Scene::Render() {
    // 1. 获取主相机
    Camera* mainCamera = CameraManager::GetInstance().GetMainCamera();
    
    // 2. 设置渲染状态
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    // 3. 收集所有渲染组件
    std::vector<RenderComponent*> renderComponents;
    CollectRenderComponents(renderComponents);
    
    // 4. 对渲染组件进行排序
    SortRenderComponents(renderComponents);
    
    // 5. 统一渲染所有组件
    for (auto component : renderComponents) {
        if (component->IsVisible()) {
            component->Render(renderer);
        }
    }
}
```

### 3. 渲染组件收集和排序

```cpp
void Scene::CollectRenderComponents(std::vector<RenderComponent*>& components) {
    // 遍历场景中的所有GameObject
    for (auto gameObject : gameObjects) {
        CollectRenderComponentsFromGameObject(gameObject, components);
    }
}

void Scene::CollectRenderComponentsFromGameObject(GameObject* gameObject, 
                                                  std::vector<RenderComponent*>& components) {
    // 1. 检查对象激活状态
    if (!gameObject->IsActive()) return;
    
    // 2. 获取GameObject上的所有渲染组件
    auto renderComponents = gameObject->GetComponents<RenderComponent>();
    
    // 3. 添加到渲染队列
    for (auto component : renderComponents) {
        if (component->IsVisible()) {
            components.push_back(component);
        }
    }
    
    // 4. 递归处理子对象
    for (auto child : gameObject->GetChildren()) {
        CollectRenderComponentsFromGameObject(child, components);
    }
}

void Scene::SortRenderComponents(std::vector<RenderComponent*>& components) {
    // 按渲染层级和排序顺序进行排序
    std::sort(components.begin(), components.end(), 
              [](const RenderComponent* a, const RenderComponent* b) {
                  // 首先按渲染层级排序
                  if (a->GetRenderLayer() != b->GetRenderLayer()) {
                      return a->GetRenderLayer() < b->GetRenderLayer();
                  }
                  // 然后按排序顺序排序
                  return a->GetSortingOrder() < b->GetSortingOrder();
              });
}
```

## 设计优势

### 1. 模块化设计
- **组件解耦**: 渲染逻辑与游戏逻辑分离
- **可扩展性**: 易于添加新的渲染组件类型
- **可维护性**: 清晰的架构层次

### 2. 性能优化
- **批量渲染**: 通过组件收集和排序，支持相同材质的批量渲染
- **渲染排序**: 通过层级和排序顺序优化渲染顺序，减少状态切换
- **视锥剔除**: 可以在收集阶段集成视锥剔除优化
- **早期剔除**: 在收集阶段就过滤掉不可见的组件

### 3. 灵活性
- **多场景支持**: 支持多个场景的切换
- **多相机支持**: 支持多相机渲染
- **材质系统**: 灵活的材质和着色器管理

## 使用示例

### 1. 创建简单场景

```cpp
class MyScene : public Scene {
public:
    void Initialize() override {
        // 创建游戏对象
        auto cubeObject = new GameObject();
        cubeObject->SetName("Cube");
        
        // 添加网格组件
        auto meshFilter = cubeObject->AddComponent<MeshFilter>();
        meshFilter->CreateMesh();
        
        // 设置网格数据
        auto mesh = meshFilter->GetMesh();
        mesh->SetVertices(cubeVertices);
        mesh->SetIndices(cubeIndices);
        
        // 添加渲染组件
        auto meshRenderer = cubeObject->AddComponent<MeshRenderer>();
        meshRenderer->SetMaterial(material);
        
        // 设置变换
        auto transform = cubeObject->GetTransform();
        transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        
        gameObjects.push_back(cubeObject);
    }
    
    void Render() override {
        // 1. 收集所有渲染组件
        std::vector<RenderComponent*> renderComponents;
        CollectRenderComponents(renderComponents);
        
        // 2. 对渲染组件进行排序
        SortRenderComponents(renderComponents);
        
        // 3. 统一渲染所有组件
        for (auto component : renderComponents) {
            if (component->IsVisible()) {
                component->Render(renderer);
            }
        }
    }
    
private:
    void CollectRenderComponents(std::vector<RenderComponent*>& components) {
        for (auto gameObject : gameObjects) {
            CollectFromGameObject(gameObject, components);
        }
    }
    
    void CollectFromGameObject(GameObject* gameObject, 
                               std::vector<RenderComponent*>& components) {
        if (!gameObject->IsActive()) return;
        
        auto renderComponents = gameObject->GetComponents<RenderComponent>();
        for (auto component : renderComponents) {
            if (component->IsVisible()) {
                components.push_back(component);
            }
        }
        
        // 递归处理子对象
        for (auto child : gameObject->GetChildren()) {
            CollectFromGameObject(child, components);
        }
    }
    
    void SortRenderComponents(std::vector<RenderComponent*>& components) {
        std::sort(components.begin(), components.end(), 
                  [](const RenderComponent* a, const RenderComponent* b) {
                      if (a->GetRenderLayer() != b->GetRenderLayer()) {
                          return a->GetRenderLayer() < b->GetRenderLayer();
                      }
                      return a->GetSortingOrder() < b->GetSortingOrder();
                  });
    }
};
```

### 2. 注册和使用场景

```cpp
void Application::InitScenes() {
    // 创建场景
    auto myScene = std::make_shared<MyScene>();
    
    // 注册场景
    SceneManager::GetInstance().AddScene("MyScene", myScene);
    
    // 设置活动场景
    SetInitialScene("MyScene");
}
```

## 扩展建议

### 1. 渲染优化
- **组件收集优化**: 支持空间分区优化，如八叉树、BSP树等
- **实例化渲染**: 支持大量相同对象的实例化渲染
- **LOD系统**: 根据距离选择合适的细节级别
- **动态批处理**: 在排序阶段自动合并相同材质的对象
- **多线程收集**: 支持多线程并行收集渲染组件

### 2. 高级特性
- **阴影系统**: 支持实时阴影渲染
- **后处理**: 支持后处理效果管道
- **多线程渲染**: 支持多线程渲染优化

### 3. 工具链
- **场景编辑器**: 可视化场景编辑工具
- **资源管理**: 自动化资源加载和管理
- **性能分析**: 渲染性能分析工具

## 总结

ZEngine的Scene和GameObject渲染系统提供了一个灵活、高效且易于扩展的渲染架构。通过组件化的设计，开发者可以轻松创建复杂的3D场景，同时保持代码的清晰性和可维护性。该系统已经支持基本的3D渲染需求，并为未来的扩展奠定了坚实的基础。
