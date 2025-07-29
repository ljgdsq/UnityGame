#include "Framework/Core/SerializableValue.h"

namespace framework
{

    rapidjson::Value SerializableValue::Serialize(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value jsonValue(rapidjson::kObjectType);
        jsonValue.AddMember("name", rapidjson::Value(m_name.c_str(), allocator), allocator);
        jsonValue.AddMember("type", rapidjson::Value(SerializableValueTypeToString(GetType()).c_str(), allocator), allocator);

        switch (GetType())
        {
            case SerializableValueType::Bool:
                jsonValue.AddMember("value", GetValue<bool>(), allocator);
                break;
            case SerializableValueType::Char:
                jsonValue.AddMember("value", std::string(1, GetValue<char>()).data()[0], allocator);
                break;
            case SerializableValueType::Int:
                jsonValue.AddMember("value", GetValue<int>(), allocator);
                break;
            case SerializableValueType::Float:
                jsonValue.AddMember("value", GetValue<float>(), allocator);
                break;
            case SerializableValueType::Double:
                jsonValue.AddMember("value", GetValue<double>(), allocator);
                break;
            case SerializableValueType::String:
                jsonValue.AddMember("value", rapidjson::Value(GetValue<std::string>().c_str(), allocator), allocator);
                break;
            case SerializableValueType::Vec2:
                {
                    glm::vec2 vec = GetValue<glm::vec2>();
                    rapidjson::Value vecArray(rapidjson::kArrayType);
                    vecArray.PushBack(vec.x, allocator);
                    vecArray.PushBack(vec.y, allocator);
                    jsonValue.AddMember("value", vecArray, allocator);
                }
                break;
            case SerializableValueType::Vec3:
                {
                    glm::vec3 vec = GetValue<glm::vec3>();
                    rapidjson::Value vecArray(rapidjson::kArrayType);
                    vecArray.PushBack(vec.x, allocator);
                    vecArray.PushBack(vec.y, allocator);
                    vecArray.PushBack(vec.z, allocator);
                    jsonValue.AddMember("value", vecArray, allocator);
                }
                break;
            case SerializableValueType::Vec4:
                {
                    glm::vec4 vec = GetValue<glm::vec4>();
                    rapidjson::Value vecArray(rapidjson::kArrayType);
                    vecArray.PushBack(vec.x, allocator);
                    vecArray.PushBack(vec.y, allocator);
                    vecArray.PushBack(vec.z, allocator);
                    vecArray.PushBack(vec.w, allocator);
                }
            }
        return jsonValue;
    }

    void SerializableValue::Deserialize(const rapidjson::Value &jsonValue)
    {
        if (jsonValue.HasMember("name") && jsonValue["name"].IsString())
        {
            m_name = jsonValue["name"].GetString();
        }

        if (jsonValue.HasMember("type") && jsonValue["type"].IsString())
        {
            auto typeStr = jsonValue["type"].GetString();
            auto type = StringToSerializableValueType(typeStr);
            switch (type)
            {
                case SerializableValueType::Bool:
                    m_value = jsonValue["value"].GetBool();
                    break;
                case SerializableValueType::Char:
                    m_value = jsonValue["value"].GetString()[0];
                    break;
                case SerializableValueType::Int:
                    m_value = jsonValue["value"].GetInt();
                    break;
                case SerializableValueType::Float:
                    m_value = jsonValue["value"].GetDouble();
                    break;
                case SerializableValueType::Double:
                    m_value = jsonValue["value"].GetDouble();
                    break;
                case SerializableValueType::String:
                    m_value = jsonValue["value"].GetString();
                    break;
                case SerializableValueType::Vec2:
                    {
                        const auto& arr = jsonValue["value"];
                        if (arr.Size() == 2)
                            m_value = glm::vec2(arr[0].GetDouble(), arr[1].GetDouble());
                        else
                            throw std::invalid_argument("Invalid Vec2 value");
                    }
                    break;
                case SerializableValueType::Vec3:
                    {
                        const auto& arr = jsonValue["value"];
                        if (arr.Size() == 3)
                            m_value = glm::vec3(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble());
                        else
                            throw std::invalid_argument("Invalid Vec3 value");
                    }
                    break;
                case SerializableValueType::Vec4:
                    {
                        const auto& arr = jsonValue["value"];
                        if (arr.Size() == 4)
                            m_value = glm::vec4(arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble(), arr[3].GetDouble());
                        else
                            throw std::invalid_argument("Invalid Vec4 value");
                    }
                    break;
            }
        }


} // namespace framework
}
