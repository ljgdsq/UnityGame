# GameObject 生命周期管理设计文档

## 概述

本文档描述了ZEngine中GameObject和Component的完整生命周期管理系统设计。当前的实现缺少完整的状态管理和生命周期控制，需要添加Enable/Disable、Created/Destroyed等状态，以及Scene对GameObject的完整生命周期管理。

## 当前实现分析

### 现有功能
- 基本的GameObject结构与组件系统
- 简单的Active/Inactive状态
- 父子层次结构管理
- 组件的OnCreate/OnDestroy回调

### 缺失功能
- 完整的GameObject状态管理（Enable/Disable、Created/Destroyed等）
- Scene级别的GameObject生命周期管理
- 组件生命周期的完整回调系统
- 延迟销毁机制
- 对象池管理

## 设计方案

### 1. GameObject状态枚举

```cpp
namespace framework {

enum class GameObjectState {
    Created,        // 对象已创建但未初始化
    Initialized,    // 对象已初始化
    Started,        // 对象已启动（第一次Update前）
    Active,         // 对象活跃状态
    Inactive,       // 对象非活跃状态
    Disabled,       // 对象被禁用
    Destroyed,      // 对象已标记为销毁
    PendingDestroy  // 等待销毁（延迟销毁）
};

enum class ComponentState {
    Created,        // 组件已创建
    Initialized,    // 组件已初始化
    Enabled,        // 组件已启用
    Started,        // 组件已启动
    Disabled,       // 组件已禁用
    Destroyed       // 组件已销毁
};

}
```

### 2. 增强的GameObject类

```cpp
namespace framework {

class GameObject {
public:
    GameObject();
    GameObject(const std::string& name);
    virtual ~GameObject();

    // 基本属性
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }
    
    // 状态管理
    GameObjectState GetState() const { return m_state; }
    bool IsActive() const { return m_state == GameObjectState::Active; }
    bool IsEnabled() const { return m_enabled; }
    bool IsDestroyed() const { return m_state == GameObjectState::Destroyed || 
                                    m_state == GameObjectState::PendingDestroy; }
    
    // 状态控制
    void SetActive(bool active);
    void SetEnabled(bool enabled);
    void Destroy();
    void DestroyImmediate();
    
    // 生命周期方法
    void Initialize();
    void Start();
    void Update(float deltaTime);
    void LateUpdate(float deltaTime);
    void OnDestroy();
    
    // 组件管理（扩展现有实现）
    template<typename T>
    T* AddComponent();
    
    template<typename T>
    T* GetComponent() const;
    
    template<typename T>
    void RemoveComponent();
    
    // 新增：获取组件的状态安全版本
    template<typename T>
    T* GetActiveComponent() const;
    
    template<typename T>
    std::vector<T*> GetActiveComponents() const;
    
    // 层次结构管理
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    GameObject* GetParent() const { return m_parent; }
    void SetParent(GameObject* parent);
    const std::vector<GameObject*>& GetChildren() const { return m_children; }
    
    // 查找方法
    GameObject* Find(const std::string& name);
    GameObject* FindInChildren(const std::string& name);
    
    // 事件回调
    std::function<void(GameObject*)> OnStateChanged;
    std::function<void(GameObject*)> OnDestroyed;

private:
    std::string m_name;
    GameObjectState m_state = GameObjectState::Created;
    bool m_enabled = true;
    bool m_started = false;
    
    Transform* m_transform = nullptr;
    std::unordered_map<std::type_index, std::vector<Component*>> m_components;
    
    std::vector<GameObject*> m_children;
    GameObject* m_parent = nullptr;
    
    Scene* m_scene = nullptr; // 所属场景
    
    // 内部方法
    void SetState(GameObjectState newState);
    void NotifyStateChanged();
    void UpdateComponentStates();
    
    friend class Scene;
};

}
```

### 3. 增强的Component基类

```cpp
namespace framework {

class Component : public Serializable {
public:
    explicit Component(GameObject* gameObject);
    virtual ~Component() = default;
    
    // 基本属性
    GameObject* GetGameObject() const { return m_gameObject; }
    GameObject* GetOwner() const { return m_gameObject; }
    
    // 状态管理
    ComponentState GetState() const { return m_state; }
    bool IsEnabled() const { return m_enabled && m_state != ComponentState::Disabled; }
    bool IsDestroyed() const { return m_state == ComponentState::Destroyed; }
    
    void SetEnabled(bool enabled);
    
    // 生命周期回调（由GameObject调用）
    void Initialize();
    void Start();
    void Update(float deltaTime);
    void LateUpdate(float deltaTime);
    void OnDestroy();
    
    // 状态变化回调
    void OnEnable();
    void OnDisable();
    
    // 类型信息
    virtual const char* GetName() const = 0;
    virtual const char* GetTypeName() const = 0;
    virtual const std::type_info& GetTypeInfo() const = 0;

protected:
    // 子类重写的生命周期方法
    virtual void OnCreate() {}
    virtual void OnInitialize() {}
    virtual void OnStart() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnLateUpdate(float deltaTime) {}
    virtual void OnDestroy_Internal() {}
    virtual void OnEnable_Internal() {}
    virtual void OnDisable_Internal() {}

private:
    GameObject* m_gameObject;
    ComponentState m_state = ComponentState::Created;
    bool m_enabled = true;
    bool m_started = false;
    
    void SetState(ComponentState newState);
    
    friend class GameObject;
};

}
```

### 4. Scene的GameObject管理

```cpp
namespace framework {

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;
    
    // 基本生命周期
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
    
    // GameObject管理
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);
    GameObject* CreateGameObject(const std::string& name = "");
    
    // 查找功能
    GameObject* FindGameObject(const std::string& name);
    std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag);
    
    // 获取所有GameObject
    const std::vector<GameObject*>& GetGameObjects() const { return m_gameObjects; }
    std::vector<GameObject*> GetActiveGameObjects() const;
    
    // 生命周期管理
    void InitializeGameObjects();
    void StartGameObjects();
    void UpdateGameObjects(float deltaTime);
    void LateUpdateGameObjects(float deltaTime);
    void DestroyPendingGameObjects();
    
    // 组件查询
    template<typename T>
    T* FindComponent();
    
    template<typename T>
    std::vector<T*> FindComponents();
    
    template<typename T>
    std::vector<T*> FindActiveComponents();

protected:
    std::vector<GameObject*> m_gameObjects;
    std::vector<GameObject*> m_pendingDestroy;
    std::vector<GameObject*> m_newGameObjects;
    
    bool m_initialized = false;
    bool m_started = false;
    
    // 内部管理方法
    void ProcessNewGameObjects();
    void ProcessPendingDestroy();
    
    // 渲染相关
    void CollectRenderComponents(std::vector<RenderComponent*>& components);
    void SortRenderComponents(std::vector<RenderComponent*>& components);
};

}
```

### 5. 生命周期管理器

```cpp
namespace framework {

class GameObjectLifecycleManager {
public:
    static GameObjectLifecycleManager& GetInstance();
    
    // 延迟销毁管理
    void DestroyDelayed(GameObject* gameObject, float delay = 0.0f);
    void DestroyImmediate(GameObject* gameObject);
    
    // 对象池管理
    GameObject* GetPooledObject(const std::string& prefabName);
    void ReturnToPool(GameObject* gameObject);
    
    // 全局查找
    GameObject* Find(const std::string& name);
    std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag);
    
    // 统计信息
    size_t GetActiveGameObjectCount() const;
    size_t GetTotalGameObjectCount() const;
    
    // 更新管理
    void Update(float deltaTime);
    
private:
    struct DelayedDestroy {
        GameObject* gameObject;
        float remainingTime;
    };
    
    std::vector<DelayedDestroy> m_delayedDestroy;
    std::unordered_map<std::string, std::vector<GameObject*>> m_objectPools;
    
    static std::unique_ptr<GameObjectLifecycleManager> s_instance;
};

}
```

## 实现细节

### 1. GameObject状态转换

```cpp
void GameObject::SetState(GameObjectState newState) {
    if (m_state == newState) return;
    
    GameObjectState oldState = m_state;
    m_state = newState;
    
    // 状态转换逻辑
    switch (newState) {
        case GameObjectState::Initialized:
            if (oldState == GameObjectState::Created) {
                Initialize();
            }
            break;
            
        case GameObjectState::Started:
            if (oldState == GameObjectState::Initialized) {
                Start();
            }
            break;
            
        case GameObjectState::Active:
            if (oldState == GameObjectState::Inactive) {
                OnEnable();
            }
            break;
            
        case GameObjectState::Inactive:
            if (oldState == GameObjectState::Active) {
                OnDisable();
            }
            break;
            
        case GameObjectState::Destroyed:
            OnDestroy();
            break;
    }
    
    // 更新组件状态
    UpdateComponentStates();
    
    // 通知状态变化
    NotifyStateChanged();
}
```

### 2. 延迟销毁机制

```cpp
void GameObject::Destroy() {
    if (IsDestroyed()) return;
    
    SetState(GameObjectState::PendingDestroy);
    
    // 将对象添加到场景的待销毁列表
    if (m_scene) {
        m_scene->AddPendingDestroy(this);
    }
}

void Scene::DestroyPendingGameObjects() {
    for (auto gameObject : m_pendingDestroy) {
        // 先销毁所有子对象
        auto children = gameObject->GetChildren();
        for (auto child : children) {
            child->DestroyImmediate();
        }
        
        // 从父对象中移除
        if (gameObject->GetParent()) {
            gameObject->GetParent()->RemoveChild(gameObject);
        }
        
        // 从场景中移除
        RemoveGameObject(gameObject);
        
        // 销毁对象
        gameObject->SetState(GameObjectState::Destroyed);
        delete gameObject;
    }
    
    m_pendingDestroy.clear();
}
```

### 3. 组件生命周期同步

```cpp
void GameObject::UpdateComponentStates() {
    for (auto& componentPair : m_components) {
        for (auto component : componentPair.second) {
            switch (m_state) {
                case GameObjectState::Initialized:
                    if (component->GetState() == ComponentState::Created) {
                        component->Initialize();
                    }
                    break;
                    
                case GameObjectState::Started:
                    if (component->GetState() == ComponentState::Initialized) {
                        component->Start();
                    }
                    break;
                    
                case GameObjectState::Active:
                    if (component->IsEnabled()) {
                        component->OnEnable();
                    }
                    break;
                    
                case GameObjectState::Inactive:
                    component->OnDisable();
                    break;
                    
                case GameObjectState::Destroyed:
                    component->OnDestroy();
                    break;
            }
        }
    }
}
```

### 4. Scene的完整更新循环

```cpp
void Scene::Update(float deltaTime) {
    // 处理新添加的GameObject
    ProcessNewGameObjects();
    
    // 启动未启动的GameObject
    if (m_started) {
        StartGameObjects();
    }
    
    // 更新所有活跃的GameObject
    UpdateGameObjects(deltaTime);
    
    // 后更新
    LateUpdateGameObjects(deltaTime);
    
    // 处理待销毁的GameObject
    DestroyPendingGameObjects();
    
    // 更新生命周期管理器
    GameObjectLifecycleManager::GetInstance().Update(deltaTime);
}

void Scene::UpdateGameObjects(float deltaTime) {
    for (auto gameObject : m_gameObjects) {
        if (gameObject->IsActive()) {
            gameObject->Update(deltaTime);
        }
    }
}
```

## 使用示例

### 1. 创建和管理GameObject

```cpp
// 在Scene中创建GameObject
void MyScene::Initialize() {
    // 创建GameObject
    auto player = CreateGameObject("Player");
    
    // 添加组件
    auto meshRenderer = player->AddComponent<MeshRenderer>();
    auto collider = player->AddComponent<BoxCollider>();
    
    // 设置属性
    player->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
    
    // 创建子对象
    auto weapon = CreateGameObject("Weapon");
    player->AddChild(weapon);
}

// 延迟销毁
void MyScene::DestroyEnemy(GameObject* enemy) {
    // 3秒后销毁
    GameObjectLifecycleManager::GetInstance().DestroyDelayed(enemy, 3.0f);
}
```

### 2. 组件生命周期使用

```cpp
class PlayerController : public ComponentBase<PlayerController> {
protected:
    void OnInitialize() override {
        // 初始化组件
        Logger::Log("Player controller initialized");
    }
    
    void OnStart() override {
        // 第一次Update前调用
        m_rigidbody = GetGameObject()->GetComponent<Rigidbody>();
    }
    
    void OnUpdate(float deltaTime) override {
        // 每帧更新
        if (Input::GetKey(GLFW_KEY_W)) {
            MoveForward(deltaTime);
        }
    }
    
    void OnEnable_Internal() override {
        // 组件启用时
        Logger::Log("Player controller enabled");
    }
    
    void OnDisable_Internal() override {
        // 组件禁用时
        Logger::Log("Player controller disabled");
    }
};
```

### 3. 对象池使用

```cpp
// 创建子弹对象池
void GameManager::Initialize() {
    auto& lifecycle = GameObjectLifecycleManager::GetInstance();
    
    // 预创建子弹对象
    for (int i = 0; i < 50; ++i) {
        auto bullet = CreateGameObject("Bullet");
        bullet->AddComponent<BulletComponent>();
        bullet->SetActive(false);
        lifecycle.ReturnToPool(bullet);
    }
}

// 使用对象池
void Gun::Fire() {
    auto& lifecycle = GameObjectLifecycleManager::GetInstance();
    auto bullet = lifecycle.GetPooledObject("Bullet");
    
    if (bullet) {
        bullet->SetActive(true);
        bullet->GetTransform()->SetPosition(m_firePoint);
        // 设置子弹属性...
    }
}
```

## 扩展功能

### 1. 事件系统集成

```cpp
class GameObject {
public:
    // 事件委托
    Event<GameObject*> OnDestroyed;
    Event<GameObject*, bool> OnActiveChanged;
    Event<GameObject*, bool> OnEnabledChanged;
    
private:
    void NotifyStateChanged() {
        switch (m_state) {
            case GameObjectState::Destroyed:
                OnDestroyed.Invoke(this);
                break;
            case GameObjectState::Active:
            case GameObjectState::Inactive:
                OnActiveChanged.Invoke(this, IsActive());
                break;
        }
    }
};
```

### 2. 标签系统

```cpp
class GameObject {
public:
    void SetTag(const std::string& tag) { m_tag = tag; }
    const std::string& GetTag() const { return m_tag; }
    bool HasTag(const std::string& tag) const { return m_tag == tag; }
    
private:
    std::string m_tag;
};
```

### 3. 预制体系统

```cpp
class Prefab {
public:
    static GameObject* Instantiate(const std::string& prefabName);
    static GameObject* Instantiate(const std::string& prefabName, const glm::vec3& position);
    static void SavePrefab(GameObject* gameObject, const std::string& prefabName);
    
private:
    static std::unordered_map<std::string, std::unique_ptr<GameObject>> s_prefabs;
};
```

## 性能优化建议

### 1. 内存管理
- 使用对象池减少频繁的new/delete操作
- 实现智能指针管理减少内存泄漏风险
- 延迟销毁机制避免在Update中立即销毁对象

### 2. 更新优化
- 只更新活跃的GameObject和组件
- 使用脏标记避免不必要的计算
- 批量处理状态变化

### 3. 查找优化
- 使用哈希表缓存常用查找结果
- 实现空间分区优化大量对象的查找
- 避免字符串比较，使用ID或哈希值

## 总结

这个设计提供了完整的GameObject和Component生命周期管理系统，包括：

1. **完整的状态管理**: 从创建到销毁的完整状态转换
2. **Scene级别的管理**: Scene负责统一管理所有GameObject的生命周期
3. **延迟销毁机制**: 避免在Update中立即销毁对象造成的问题
4. **对象池支持**: 提高性能，减少内存分配
5. **事件系统**: 支持状态变化的事件通知
6. **扩展性**: 易于添加新的功能和优化

这个系统可以逐步实现，先实现基本的状态管理，然后逐步添加高级功能。
