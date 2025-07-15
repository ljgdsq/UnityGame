# ZEngine资源系统架构重构设计

## 概述

本文档描述了ZEngine资源系统的架构重构设计，旨在解决当前系统中职责重复、性能低下、编辑器集成困难等问题。

## 当前问题分析

### 1. 职责重复问题
- **Asset类**包含`Load()`方法，负责自身加载逻辑
- **AssetLoader类**也包含`LoadAsset()`方法，造成职责重复
- 违反了单一职责原则

### 2. 性能问题
- 每次运行都需要解析原始文件格式(.png, .obj等)
- 缺乏有效的缓存机制
- 运行时依赖多种文件格式解析库

### 3. 编辑器集成问题
- ContentBrowser显示原始文件而非Asset
- 无法保存编辑器中的修改
- 缺乏统一的Asset管理界面

## 重构目标

### 1. 职责分离
- **Asset类**：纯数据容器，只负责数据存储和访问
- **AssetImporter类**：负责将原始文件转换为Asset
- **AssetDatabase类**：负责Asset的持久化和管理

### 2. 性能优化
- 导入时一次性转换原始文件
- 运行时直接加载优化后的Asset格式
- 实现缓存和增量更新机制

### 3. 编辑器友好
- ContentBrowser显示Asset而非原始文件
- 支持Asset的重命名、移动、删除
- 提供统一的Asset管理界面

## 架构设计

### 1. 核心类设计

#### 1.1 Asset基类（重构后）
```cpp
class Asset {
public:
    // 基本属性
    const std::string& GetName() const;
    const std::string& GetAssetId() const;
    AssetType GetType() const;

    // 状态管理
    LoadState GetLoadState() const;
    void SetLoadState(LoadState state);

    // 生命周期管理
    virtual void Unload() = 0;  // 只负责释放内存
    virtual long GetSize() const = 0;

    // 序列化（仅元数据）
    virtual rapidjson::Value SerializeMetadata(...) const;
    virtual void DeserializeMetadata(const rapidjson::Value& json);

    // 移除Load()方法！Asset不再负责加载
};
```

#### 1.2 AssetImporter类
```cpp
class AssetImporter {
public:
    virtual ~AssetImporter() = default;

    // 判断是否可以导入该文件
    virtual bool CanImport(const std::string& filePath) const = 0;

    // 导入文件并创建Asset
    virtual std::shared_ptr<Asset> Import(const std::string& filePath) = 0;

    // 获取支持的文件扩展名
    virtual std::vector<std::string> GetSupportedExtensions() const = 0;

    // 获取导入器名称
    virtual std::string GetName() const = 0;

    // 获取导入的Asset类型
    virtual AssetType GetAssetType() const = 0;
};
```

#### 1.3 AssetDatabase类
```cpp
class AssetDatabase {
public:
    static AssetDatabase& GetInstance();

    // 导入管理
    void ImportAsset(const std::string& sourcePath);
    void ReimportAsset(const std::string& assetId);
    void RefreshAssets();

    // Asset管理
    std::shared_ptr<Asset> LoadAsset(const std::string& assetId);
    void SaveAsset(const std::shared_ptr<Asset>& asset);
    bool DeleteAsset(const std::string& assetId);

    // 查询功能
    std::vector<AssetInfo> GetAllAssets() const;
    AssetInfo GetAssetInfo(const std::string& assetId) const;
    bool AssetExists(const std::string& assetId) const;

    // 依赖管理
    std::vector<std::string> GetDependencies(const std::string& assetId) const;
    std::vector<std::string> GetReferences(const std::string& assetId) const;

private:
    void RegisterImporter(std::shared_ptr<AssetImporter> importer);
    AssetImporter* FindImporter(const std::string& filePath) const;
};
```

### 2. 具体Asset类设计

#### 2.1 TextureAsset（重构后）
```cpp
class TextureAsset : public Asset {
public:
    TextureAsset(const std::string& name, const std::string& assetId);

    // 数据访问
    std::shared_ptr<Texture> GetTexture() const;
    void SetTexture(std::shared_ptr<Texture> texture);

    // 纹理信息
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;

    // Asset接口实现
    void Unload() override;
    long GetSize() const override;

    // 移除LoadFromFile()等加载方法

private:
    std::shared_ptr<Texture> m_texture;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};
```

#### 2.2 MeshAsset（重构后）
```cpp
class MeshAsset : public Asset {
public:
    MeshAsset(const std::string& name, const std::string& assetId);

    // 数据访问
    std::shared_ptr<Mesh> GetMesh() const;
    void SetMesh(std::shared_ptr<Mesh> mesh);

    // 统计信息
    long GetVertexCount() const;
    long GetTriangleCount() const;

    // Asset接口实现
    void Unload() override;
    long GetSize() const override;

    // 移除LoadOBJ()等加载方法

private:
    std::shared_ptr<Mesh> m_mesh;
    long m_vertexCount = 0;
    long m_triangleCount = 0;
};
```

### 3. 具体Importer类设计

#### 3.1 TextureImporter
```cpp
class TextureImporter : public AssetImporter {
public:
    bool CanImport(const std::string& filePath) const override;
    std::shared_ptr<Asset> Import(const std::string& filePath) override;
    std::vector<std::string> GetSupportedExtensions() const override;
    std::string GetName() const override;
    AssetType GetAssetType() const override;

private:
    bool LoadImageData(const std::string& filePath,
                      unsigned char*& data,
                      int& width, int& height, int& channels);
    std::shared_ptr<Texture> CreateTexture(unsigned char* data,
                                         int width, int height, int channels);
};
```

#### 3.2 MeshImporter
```cpp
class MeshImporter : public AssetImporter {
public:
    bool CanImport(const std::string& filePath) const override;
    std::shared_ptr<Asset> Import(const std::string& filePath) override;
    std::vector<std::string> GetSupportedExtensions() const override;
    std::string GetName() const override;
    AssetType GetAssetType() const override;

private:
    bool LoadOBJ(const std::string& filePath, std::vector<Vertex>& vertices,
                std::vector<unsigned int>& indices);
    std::shared_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices,
                                   const std::vector<unsigned int>& indices);
};
```

## 工作流程

### 1. 项目导入流程
```
1. 用户将原始文件放入Res目录
2. AssetDatabase检测到文件变化
3. 找到合适的AssetImporter
4. AssetImporter解析原始文件
5. 创建对应的Asset对象
6. 保存Asset到Assets目录（.zasset格式）
7. 更新AssetDatabase索引
```

### 2. 运行时加载流程
```
1. 请求加载Asset（通过AssetId）
2. AssetDatabase查找Asset信息
3. 从Assets目录加载.zasset文件
4. 反序列化Asset对象
5. 返回Asset给调用者
```

### 3. 编辑器显示流程
```
1. ContentBrowser扫描Assets目录
2. 显示.zasset文件而非原始文件
3. 显示Asset缩略图和元数据
4. 支持Asset的重命名、移动、删除操作
```

## 目录结构

```
Project/
├── Res/                    # 原始文件（导入源）
│   ├── Textures/
│   │   ├── grass.png
│   │   └── stone.jpg
│   └── Models/
│       ├── player.obj
│       └── enemy.fbx
├── Assets/                 # 导入后的Asset文件
│   ├── Textures/
│   │   ├── grass.zasset
│   │   └── stone.zasset
│   └── Models/
│       ├── player.zasset
│       └── enemy.zasset
├── Library/               # 缓存和临时文件
│   ├── AssetDatabase.db   # Asset数据库
│   ├── ImportCache/       # 导入缓存
│   └── Thumbnails/        # 缩略图缓存
└── ProjectSettings/       # 项目设置
    └── AssetImportSettings.json
```

## 文件格式设计

### 1. .zasset文件格式
```json
{
    "header": {
        "version": "1.0",
        "type": "texture",
        "id": "texture_001",
        "name": "grass",
        "created": "2025-01-01T00:00:00Z",
        "modified": "2025-01-01T00:00:00Z"
    },
    "metadata": {
        "source_file": "Res/Textures/grass.png",
        "source_hash": "abcdef123456",
        "import_settings": {
            "generate_mipmaps": true,
            "srgb": true,
            "compression": "dxt5"
        }
    },
    "data": {
        "width": 1024,
        "height": 1024,
        "channels": 4,
        "format": "rgba8",
        "texture_data": "binary_data_reference"
    }
}
```

### 2. AssetDatabase.db结构
```sql
CREATE TABLE assets (
    id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    type INTEGER NOT NULL,
    file_path TEXT NOT NULL,
    source_file TEXT,
    source_hash TEXT,
    created_time INTEGER,
    modified_time INTEGER
);

CREATE TABLE dependencies (
    asset_id TEXT,
    depends_on TEXT,
    FOREIGN KEY(asset_id) REFERENCES assets(id),
    FOREIGN KEY(depends_on) REFERENCES assets(id)
);
```

## 实现计划

### 阶段1：基础架构
1. 重构Asset基类，移除加载逻辑
2. 创建AssetImporter基类
3. 实现AssetDatabase基本功能
4. 创建基本的文件格式(.zasset)

### 阶段2：核心Importer
1. 实现TextureImporter
2. 实现MeshImporter
3. 测试导入流程
4. 完善错误处理

### 阶段3：编辑器集成
1. 修改ContentBrowser显示Asset
2. 实现Asset的重命名、删除功能
3. 添加导入设置界面
4. 实现缩略图生成

### 阶段4：优化和扩展
1. 实现增量导入
2. 添加依赖管理
3. 优化加载性能
4. 添加更多Asset类型支持

## 优势分析

### 1. 性能提升
- 导入时一次性转换，运行时直接加载
- 统一的Asset格式，加载速度更快
- 有效的缓存机制

### 2. 架构清晰
- 职责分离明确
- 易于扩展新的Asset类型
- 符合SOLID原则

### 3. 编辑器友好
- 统一的Asset管理界面
- 支持Asset的各种操作
- 提供丰富的元数据信息

### 4. 可维护性
- 代码结构清晰
- 易于测试
- 易于调试

## 风险和挑战

### 1. 迁移复杂性
- 需要重构大量现有代码
- 可能影响现有功能
- 需要充分测试

### 2. 存储空间
- .zasset文件可能占用更多空间
- 需要考虑压缩和优化

### 3. 兼容性
- 需要保持向后兼容
- 版本升级问题

## 总结

这个重构设计解决了当前资源系统的主要问题，提供了一个现代化的、高性能的、编辑器友好的资源管理系统。通过职责分离、导入机制和统一的Asset格式，能够显著提升开发效率和运行时性能。
