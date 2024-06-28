#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "data/Serializer.h"

using namespace rapidjson;

#include <iostream>

using std::string;

class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    explicit Vec3(float v) : Vec3(v, v, v) {}
};

class Student {
public:
    int id = 0;
    string name = "";
    float height = 0;
    bool man= false;
    Vec3 pos = Vec3(0, 0, 0);

};




template<>
void Serializer::serialize(Value &value, const char *name, const Student &v) {
    Value stu(kObjectType);
    Serializer serializer;
    serializer.serialize(stu, "id", v.id);
    serializer.serialize(stu, "name", v.name);
    serializer.serialize(stu, "height", v.height);
    serializer.serialize(stu, "pos", v.pos);
    serializer.serialize(stu, "man", v.man);
    value.AddMember(StringRef(name), stu, allocator);
}



template<>
Student Serializer::deserialize(const Value &value, const char *name) {
    Student student;
    if (value.HasMember(name) && value[name].IsObject()) {
        const Value& stu = value[name];
        Serializer serializer;
        student.id = serializer.deserialize<int>(stu, "id");
        student.name = serializer.deserialize<string>(stu, "name");
        student.height = serializer.deserialize<float>(stu, "height");
        student.man = serializer.deserialize<bool>(stu, "man");
        student.pos = serializer.deserialize<Vec3>(stu, "pos");
    }
    return student;
}


int main() {
    Document document;
    document.SetObject();

    Student student;
    student.id = 99;
    student.name = "zxx";
    student.height = 175.1;
    student.pos = {1, 2, 3};
    student.man= true;


    Serializer serializer;
    serializer.serialize(document, "student", student);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    std::cout << buffer.GetString() << std::endl;

    Student stu=serializer.deserialize<Student>(document,"student");




    return 0;
}

