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


    std::string SerializableValueTypeToString(SerializableValueType type)
    {
        switch (type)
        {
            case SerializableValueType::Bool: return "Bool";
            case SerializableValueType::Char: return "Char";
            case SerializableValueType::Int: return "Int";
            case SerializableValueType::Float: return "Float";
            case SerializableValueType::Double: return "Double";
            case SerializableValueType::String: return "String";
            case SerializableValueType::Vec2: return "Vec2";
            case SerializableValueType::Vec3: return "Vec3";
            case SerializableValueType::Vec4: return "Vec4";
            default: return "Unknown";
        }
    }

    // 将字符串转换为 SerializableValueType
    SerializableValueType StringToSerializableValueType(const std::string &typeStr)
    {
        if (typeStr == "Bool") return SerializableValueType::Bool;
        if (typeStr == "Char") return SerializableValueType::Char;
        if (typeStr == "Int") return SerializableValueType::Int;
        if (typeStr == "Float") return SerializableValueType::Float;
        if (typeStr == "Double") return SerializableValueType::Double;
        if (typeStr == "String") return SerializableValueType::String;
        if (typeStr == "Vec2") return SerializableValueType::Vec2;
        if (typeStr == "Vec3") return SerializableValueType::Vec3;
        if (typeStr == "Vec4") return SerializableValueType::Vec4;

        throw std::invalid_argument("Unknown SerializableValueType: " + typeStr);
    }


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
