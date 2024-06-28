//
// Created by zhengshulin on 2024/6/27.
//

#include "Serializer.h"
MemoryPoolAllocator<CrtAllocator> Serializer::allocator;

template<>
void Serializer::serialize(Value &value, const char *name, const string &v) {
    Value strValue;
    strValue.SetString(v.c_str(), static_cast<SizeType>(v.length()), allocator);
    value.AddMember(StringRef(name), strValue, allocator);

//    value.AddMember(StringRef(name), StringRef(v.c_str()), allocator);
}


template<>
int Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name)&&value[name].IsInt()){
        return value[name].GetInt();
    }
    return 0;
}

template<>
bool Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name)&&value[name].IsBool()){
        return value[name].GetBool();
    }
    return false;
}


template<>
float Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name) && value[name].IsFloat()) {
        return value[name].GetFloat();
    }
    return 0.0f;
}

template<>
double Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name) && value[name].IsDouble()) {
        return value[name].GetDouble();
    }
    return 0.0;
}

template<>
string Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name) && value[name].IsString()) {
        return value[name].GetString();
    }
    return "";
}


template<>
void Serializer::serialize(Value &value, const char *name, const vec3 &v) {
    Value vec(kArrayType);
    vec.PushBack(v.x, allocator);
    vec.PushBack(v.y, allocator);
    vec.PushBack(v.z, allocator);
    value.AddMember(StringRef(name), vec, allocator);
}

template<>
vec3 Serializer::deserialize(const Value &value, const char *name) {
    if (value.HasMember(name) && value[name].IsArray() && value[name].Size() == 3) {
        const Value& vec = value[name];
        return vec3(vec[0].GetFloat(), vec[1].GetFloat(), vec[2].GetFloat());
    }
    return vec3(0.0f);
}