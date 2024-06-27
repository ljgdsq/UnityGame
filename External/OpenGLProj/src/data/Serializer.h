//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_SERIALIZER_H
#define OPENGLPROJ_SERIALIZER_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
using std::string;

using namespace rapidjson;

class Serializer {
public:
    static MemoryPoolAllocator<CrtAllocator> allocator;

    template<class T>
    void serialize(Value &value, const char *name, const T &v) {
        value.AddMember(StringRef(name), v, allocator);
    }

    template<class T>
    T deserialize(const Value&value, const char *name) ;

};
MemoryPoolAllocator<CrtAllocator> Serializer::allocator;

template<>
void Serializer::serialize(Value &value, const char *name, const string &v) {
    value.AddMember(StringRef(name), StringRef(v.c_str()), allocator);
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



#endif //OPENGLPROJ_SERIALIZER_H
