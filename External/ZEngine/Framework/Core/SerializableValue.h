#pragma once
#include <string>
#include <variant>
#include <stdexcept>
#include "Framework/Core/Serializable.h"
#include "glm/glm.hpp"
namespace framework
{
    using SerializableVariant =
    std::variant<bool,char,int,float,double,std::string,glm::vec2,glm::vec3,glm::vec4>;

    enum class SerializableValueType
    {
        Bool,
        Char,
        Int,
        Float,
        Double,
        String,
        Vec2,
        Vec3,
        Vec4
    };

    std::string SerializableValueTypeToString(SerializableValueType type);

    // 将字符串转换为 SerializableValueType
    SerializableValueType StringToSerializableValueType(const std::string &typeStr);

    // 可序列化值
    class SerializableValue : public Serializable
    {
    public:
        template<typename T>
        SerializableValue(const std::string &name, T value)
            : m_name(name), m_value(value)
        {
        }

        SerializableValue() = default;

        template <typename T>
        T GetValue() const
        {
            return std::get<T>(m_value);
        }

        template<typename T>
        void SetValue(const T &value)
        {
            m_value = value;
        }

        SerializableValueType GetType() const
        {
            return static_cast<SerializableValueType>(m_value.index());
        }

        const std::string &GetName() const
        {
            return m_name;
        }

        // 序列化为JSON值
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const;

        // 从JSON值反序列化
        virtual void Deserialize(const rapidjson::Value &jsonValue);

    private:
        std::string m_name;
        SerializableVariant m_value;
    };
} // namespace framework
