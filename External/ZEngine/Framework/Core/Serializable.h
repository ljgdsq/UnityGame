#pragma once
#include "rapidjson/document.h"

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual rapidjson::Value Serialize() const = 0;
    virtual void Deserialize() = 0;
};