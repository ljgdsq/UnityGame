#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#include <iostream>
using std::string;

class Vec3{
public:
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    explicit Vec3(float v) : Vec3(v,v,v) {}
};

class Student{
public:
    int id=0;
    string name="";
    float height=0;
    Vec3 pos=Vec3(0,0,0);

};
static MemoryPoolAllocator<CrtAllocator> allocator;
template<class T>
Value SerializeT(const T&t);

template<class T>
T Deserialize(const Value&v);


template<>
Value SerializeT(const Vec3&v){
    Value value(kArrayType);
    value.PushBack(v.x, allocator);
    value.PushBack(v.y, allocator);
    value.PushBack(v.z, allocator);
    return value;
}

template<>
Vec3 Deserialize(const Value&value){
    if (value.IsArray() && value.Size() == 3) {
        return Vec3(value[0].GetFloat(), value[1].GetFloat(), value[2].GetFloat());
    } else {
        return Vec3(0.0f);
    }
}

template<>
Student Deserialize(const Value&value){
    Student student;
    // 检查并解析 id 字段
    if (value.HasMember("id") && value["id"].IsInt())
        student.id = value["id"].GetInt();
    else
        student.id = 0;  // 默认值或错误处理

    // 检查并解析 name 字段
    if (value.HasMember("name") && value["name"].IsString())
        student.name = value["name"].GetString();
    else
        student.name = "";  // 默认值或错误处理

    // 检查并解析 height 字段
    if (value.HasMember("height") && value["height"].IsFloat())
        student.height = value["height"].GetFloat();
    else
        student.height = 0.0f;  // 默认值或错误处理

    if (value.HasMember("pos") && value["pos"].IsArray()){
        const Value& posValue = value["pos"];
        student.pos = Deserialize<Vec3>(posValue);
    }
    return student;
}

template<>
Value SerializeT(const Student&v){
    Value value(kObjectType);
    value.AddMember("id",v.id,allocator);
    value.AddMember("name",StringRef(v.name.c_str()),allocator);
    value.AddMember("height",v.height,allocator);
    value.AddMember("pos",SerializeT(v.pos),allocator);
    return value;
}




int main() {
    Document document;
    document.SetObject();
    allocator = document.GetAllocator();

    Student student;
    student.id=99;
    student.name="zxx";
    student.height=175.1;
    student.pos={1,2,3};

    auto v=SerializeT(student);

    document.AddMember("person",v,allocator);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    std::cout<<buffer.GetString()<<std::endl;
    const Value& person = document["person"];
    Student stu=Deserialize<Student>(person);
    return 0;
}

