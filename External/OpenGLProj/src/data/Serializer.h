//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_SERIALIZER_H
#define OPENGLPROJ_SERIALIZER_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include "core/common.h"
using std::string;

using namespace rapidjson;

class Serializer {
public:
    static MemoryPoolAllocator<CrtAllocator> allocator;
    virtual void serialize(){};
public:
    template<class T>
    void serialize(Value &value, const char *name, const T &v) {
        value.AddMember(StringRef(name), v, allocator);
    }


    template<class T>
    void serialize(Value &value, const T &v);

    template<class T>
    T deserialize(const Value&value, const char *name) ;

};


template<>
void Serializer::serialize(Value &value, const char *name, const string &v);


template<>
int Serializer::deserialize(const Value &value, const char *name);

template<>
bool Serializer::deserialize(const Value &value, const char *name);


template<>
float Serializer::deserialize(const Value &value, const char *name);
template<>
double Serializer::deserialize(const Value &value, const char *name);

template<>
string Serializer::deserialize(const Value &value, const char *name);

template<>
void Serializer::serialize(Value &value, const char *name, const vec3 &v);
template<>
vec3 Serializer::deserialize(const Value &value, const char *name);

#endif //OPENGLPROJ_SERIALIZER_H
