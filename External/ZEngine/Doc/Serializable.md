# SerializableValue 系统设计文档

## 概述

SerializableValue 系统用于在 Inspector 面板上统一管理和序列化各种类型的参数（如 int、float、bool、string、vec3、vec4 等），支持保存为 JSON 并可加载还原。该系统基于 C++17 的 `std::variant`，类型安全且高效。

---

## 1. 支持的数据类型

```cpp
using SerializableVariant = std::variant<
    bool,
    int,
    float,
    double,
    std::string,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat3,
    glm::mat4
>;
```

---

## 2. 类型枚举

```cpp
enum class ValueType
{
    Bool,
    Int,
    Float,
    Double,
    String,
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4
};
```

---

## 3. SerializableValue 类接口

```cpp
class SerializableValue
{
public:
    SerializableValue();
    template<typename T>
    SerializableValue(const std::string& name, const T& value, const std::string& displayName = "");

    template<typename T>
    T GetValue() const;

    template<typename T>
    void SetValue(const T& value);

    template<typename T>
    bool IsType() const;

    ValueType GetType() const;
    std::string GetTypeName() const;

    const std::string& GetName() const;
    const std::string& GetDisplayName() const;

    // JSON 序列化/反序列化
    rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const;
    bool Deserialize(const rapidjson::Value& jsonValue);

    // 字符串转换
    std::string ToString() const;
    bool FromString(const std::string& str);

private:
    SerializableVariant m_value;
    std::string m_name;
    std::string m_displayName;
};
```

---

## 4. MaterialProperties 容器

```cpp
class MaterialProperties
{
public:
    template<typename T>
    void SetProperty(const std::string& name, const T& value, const std::string& displayName = "");

    template<typename T>
    T GetProperty(const std::string& name, const T& defaultValue = T{}) const;

    bool HasProperty(const std::string& name) const;

    const std::unordered_map<std::string, SerializableValue>& GetAllProperties() const;

    rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const;
    bool Deserialize(const rapidjson::Value& jsonValue);

private:
    std::unordered_map<std::string, SerializableValue> m_properties;
};
```

---

## 5. JSON 示例

```json
{
  "name": "roughness",
  "displayName": "表面粗糙度",
  "type": "float",
  "value": 0.5
}
```

```json
{
  "name": "albedo",
  "displayName": "反照率颜色",
  "type": "vec3",
  "value": [1.0, 0.0, 0.0]
}
```

---

## 6. Inspector 面板集成建议

- 遍历 MaterialProperties，按类型动态生成 UI 控件（如滑块、输入框、颜色选择器等）。
- 支持范围（min/max）和默认值。
- 支持参数的序列化和反序列化，便于保存和还原材质参数。

---

## 7. 为什么选用 std::variant

- **类型安全**：编译期检查，避免运行时类型错误。
- **性能优良**：无堆分配，栈上存储。
- **易于扩展**：只需在 variant 类型列表中添加新类型即可。

---

## 8. 扩展建议

- 支持更多类型（如纹理、颜色等）。
- 支持参数分组与排序。
- Inspector 支持参数的动态增删和重命名。

---

如需详细实现代码，请参考 `Framework/Core/SerializableValue.h` 及相关源文件。
