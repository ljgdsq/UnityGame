#pragma once
#include "rapidjson/document.h"

class Serializable
{
public:
    virtual ~Serializable() = default;

    // 序列化为JSON值
    virtual rapidjson::Value Serialize() const = 0;

    // 从JSON值反序列化
    virtual void Deserialize(const rapidjson::Value &jsonValue) = 0;
};