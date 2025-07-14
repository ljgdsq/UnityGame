# ZEngine Asset Loading Strategy & Workflow

## 1. 资源加载策略

### 1.1 分层加载策略

**不应该一开始就加载所有资源**，而是采用以下分层策略：

```
┌─────────────────────────────────────────────────────────────┐
│                   Asset Loading Strategy                     │
└─────────────────────────────────────────────────────────────┘

Level 1: Asset Metadata (启动时加载)
├── AssetRegistry: 资源注册表
├── AssetInfo: 资源元信息
└── File System Scan: 文件系统扫描

Level 2: Asset Proxy (按需创建)
├── AssetReference: 资源引用
├── Asset Placeholder: 资源占位符
└── Lazy Loading: 懒加载机制

Level 3: Asset Data (使用时加载)
├── Texture Data: 纹理数据
├── Mesh Data: 网格数据
└── Material Data: 材质数据

Level 4: GPU Resources (渲染时上传)
├── OpenGL Textures: GPU纹理
├── Vertex Buffers: 顶点缓冲
└── Index Buffers: 索引缓冲
```

### 1.2 加载时机分类

1. **启动时加载（Startup Loading）**

   - 资源元数据和目录结构
   - 必要的默认资源（默认材质、默认纹理等）
   - 引擎核心资源

2. **场景加载时（Scene Loading）**

   - 场景中引用的 Asset 元信息
   - 创建 AssetReference 但不加载实际数据
   - 预加载关键资源

3. **按需加载（On-Demand Loading）**

   - 首次使用时加载实际资源数据
   - 渲染前加载 GPU 资源
   - 异步加载非关键资源

4. **预加载（Preloading）**
   - 预测即将使用的资源
   - 后台异步加载
   - 内存池管理

## 2. 详细工作流程

### 2.1 引擎启动流程

```cpp
// 1. 引擎启动时的Asset系统初始化
void AssetSystem::Initialize() {
    // 1.1 扫描资源目录，建立元数据索引
    AssetRegistry::GetInstance().ScanResourceDirectories();

    // 1.2 加载资源清单文件
    LoadAssetManifest("Assets/AssetManifest.json");

    // 1.3 预加载必要的默认资源
    PreloadEssentialAssets();

    // 1.4 启动异步加载线程
    StartAsyncLoadingThread();
}

void AssetRegistry::ScanResourceDirectories() {
    // 扫描所有资源文件，但不加载实际数据
    for (const auto& dir : GetResourceDirectories()) {
        ScanDirectory(dir);
    }
}

void AssetRegistry::ScanDirectory(const std::string& directory) {
    for (const auto& file : filesystem::directory_iterator(directory)) {
        if (IsAssetFile(file.path())) {
            // 只创建AssetInfo，不加载实际数据
            AssetInfo info;
            info.assetId = GenerateAssetId(file.path());
            info.filePath = file.path();
            info.type = DeduceAssetType(file.path());
            info.lastModified = filesystem::last_write_time(file.path());

            RegisterAssetInfo(info);
        }
    }
}
```

### 2.2 场景加载流程

```cpp
// 2. 场景加载时的Asset处理
void Scene::LoadFromFile(const std::string& scenePath) {
    // 2.1 解析场景文件
    auto sceneJson = LoadSceneJson(scenePath);

    // 2.2 收集场景中的所有Asset引用
    std::vector<std::string> referencedAssets;
    CollectAssetReferences(sceneJson, referencedAssets);

    // 2.3 创建AssetReference对象（不加载实际数据）
    for (const auto& assetId : referencedAssets) {
        AssetManager::GetInstance().CreateAssetReference(assetId);
    }

    // 2.4 异步预加载关键资源
    AssetManager::GetInstance().PreloadAssets(referencedAssets, LoadPriority::High);

    // 2.5 创建GameObject和Component
    CreateGameObjects(sceneJson);
}

void GameObject::Deserialize(const rapidjson::Value& json) {
    // 组件反序列化时创建AssetReference
    if (json.HasMember("MeshFilter")) {
        auto meshFilter = AddComponent<MeshFilter>();

        // 从JSON中获取Asset ID，创建引用但不立即加载
        std::string meshAssetId = json["MeshFilter"]["meshAssetId"].GetString();
        meshFilter->SetMeshAssetId(meshAssetId); // 只设置ID，不加载
    }
}
```

### 2.3 运行时加载流程

```cpp
// 3. 运行时按需加载
void MeshFilter::GetMesh() const {
    // 首次访问时触发加载
    if (!m_meshAssetRef.IsLoaded()) {
        m_meshAssetRef.Load(); // 异步加载
    }

    return m_meshAssetRef.Get()->GetMesh();
}

void AssetReference<MeshAsset>::Load() {
    if (IsLoaded()) return;

    // 异步加载Asset
    AssetManager::GetInstance().LoadAssetAsync(m_assetId, [this](std::shared_ptr<Asset> asset) {
        m_cachedAsset = std::static_pointer_cast<MeshAsset>(asset);
        m_loadState = LoadState::Loaded;
    });
}
```

## 3. 具体实现设计

### 3.1 AssetInfo 轻量级元数据

```cpp
struct AssetInfo {
    std::string assetId;           // 唯一标识
    std::string displayName;       // 显示名称
    std::string filePath;         // 文件路径
    AssetType type;               // 资源类型
    size_t fileSize;              // 文件大小
    std::time_t lastModified;     // 最后修改时间

    // 缩略图信息（延迟生成）
    std::string thumbnailPath;
    void* thumbnailTextureId = nullptr;

    // 加载状态
    LoadState loadState = LoadState::NotLoaded;
    LoadPriority priority = LoadPriority::Normal;

    // 依赖关系
    std::vector<std::string> dependencies;
    std::vector<std::string> dependents;
};
```

### 3.2 AssetReference 懒加载机制

```cpp
template<typename AssetType>
class AssetReference {
public:
    // 构造时只记录ID，不加载
    AssetReference(const std::string& assetId)
        : m_assetId(assetId), m_loadState(LoadState::NotLoaded) {}

    // 访问时触发加载
    std::shared_ptr<AssetType> Get() const {
        if (m_loadState == LoadState::NotLoaded) {
            RequestLoad();
        }

        // 返回缓存的资源或默认资源
        if (auto asset = m_cachedAsset.lock()) {
            return asset;
        }

        // 返回默认资源防止空指针
        return GetDefaultAsset<AssetType>();
    }

    // 异步加载请求
    void RequestLoad() const {
        if (m_loadState != LoadState::NotLoaded) return;

        m_loadState = LoadState::Loading;

        AssetManager::GetInstance().LoadAssetAsync(m_assetId,
            [this](std::shared_ptr<Asset> asset) {
                m_cachedAsset = std::static_pointer_cast<AssetType>(asset);
                m_loadState = LoadState::Loaded;
            });
    }

private:
    std::string m_assetId;
    mutable std::weak_ptr<AssetType> m_cachedAsset;
    mutable LoadState m_loadState;
};
```

### 3.3 异步加载系统

```cpp
class AssetLoadingSystem {
public:
    void Initialize() {
        // 创建加载线程池
        m_loadingThreads.resize(std::thread::hardware_concurrency());
        for (auto& thread : m_loadingThreads) {
            thread = std::thread(&AssetLoadingSystem::LoadingWorker, this);
        }
    }

    void RequestAssetLoad(const std::string& assetId, LoadPriority priority,
                         std::function<void(std::shared_ptr<Asset>)> callback) {
        LoadRequest request;
        request.assetId = assetId;
        request.priority = priority;
        request.callback = callback;

        // 加入优先队列
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_loadQueue.push(request);
        }
        m_queueCondition.notify_one();
    }

private:
    void LoadingWorker() {
        while (m_running) {
            LoadRequest request;

            // 从队列中获取加载请求
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_queueCondition.wait(lock, [this] { return !m_loadQueue.empty() || !m_running; });

                if (!m_running) break;

                request = m_loadQueue.top();
                m_loadQueue.pop();
            }

            // 执行加载
            auto asset = LoadAssetSync(request.assetId);

            // 回调通知
            if (request.callback) {
                request.callback(asset);
            }
        }
    }

    std::priority_queue<LoadRequest> m_loadQueue;
    std::vector<std::thread> m_loadingThreads;
    std::mutex m_queueMutex;
    std::condition_variable m_queueCondition;
    bool m_running = true;
};
```

## 4. 内存管理策略

### 4.1 LRU 缓存系统

```cpp
class AssetCache {
public:
    void SetCacheSize(size_t maxSize) { m_maxCacheSize = maxSize; }

    void CacheAsset(const std::string& assetId, std::shared_ptr<Asset> asset) {
        // 检查缓存大小
        if (m_currentCacheSize + asset->GetSize() > m_maxCacheSize) {
            EvictLeastRecentlyUsed();
        }

        // 添加到缓存
        m_cache[assetId] = asset;
        m_accessOrder.push_front(assetId);
        m_currentCacheSize += asset->GetSize();
    }

    std::shared_ptr<Asset> GetAsset(const std::string& assetId) {
        auto it = m_cache.find(assetId);
        if (it != m_cache.end()) {
            // 更新访问顺序
            UpdateAccessOrder(assetId);
            return it->second;
        }
        return nullptr;
    }

private:
    void EvictLeastRecentlyUsed() {
        while (m_currentCacheSize > m_maxCacheSize * 0.8f && !m_accessOrder.empty()) {
            auto lru = m_accessOrder.back();
            m_accessOrder.pop_back();

            auto it = m_cache.find(lru);
            if (it != m_cache.end()) {
                m_currentCacheSize -= it->second->GetSize();
                m_cache.erase(it);
            }
        }
    }

    std::unordered_map<std::string, std::shared_ptr<Asset>> m_cache;
    std::list<std::string> m_accessOrder;
    size_t m_maxCacheSize = 512 * 1024 * 1024; // 512MB
    size_t m_currentCacheSize = 0;
};
```

## 5. 完整的工作流程总结

### 5.1 启动阶段

```
引擎启动
├── 扫描资源目录 (只获取文件信息)
├── 建立AssetRegistry (轻量级元数据)
├── 预加载必要资源 (默认材质、默认纹理)
└── 启动异步加载线程池
```

### 5.2 场景加载阶段

```
场景加载
├── 解析场景文件
├── 收集Asset引用ID
├── 创建AssetReference对象 (不加载实际数据)
├── 异步预加载关键资源
└── 创建GameObject和Component
```

### 5.3 运行时阶段

```
运行时访问
├── 组件请求Asset (如MeshFilter.GetMesh())
├── AssetReference检查加载状态
├── 触发异步加载 (如果未加载)
├── 返回默认资源 (加载完成前)
└── 回调更新引用 (加载完成后)
```

## 6. 性能优化建议

### 6.1 预加载策略

- **场景预加载**：加载当前场景的所有资源
- **邻近预加载**：预测玩家可能访问的区域
- **时间切片**：分帧加载避免卡顿

### 6.2 内存管理

- **引用计数**：自动释放未使用资源
- **LRU 缓存**：限制内存使用
- **分级存储**：内存 -> 磁盘 -> 网络

### 6.3 加载优化

- **批量加载**：合并小文件请求
- **压缩传输**：减少 IO 时间
- **增量更新**：只更新修改的资源

这种分层加载策略既保证了启动性能，又提供了良好的用户体验，是现代游戏引擎的标准做法。
