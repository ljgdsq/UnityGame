//
// Created by zhengshulin on 2024/6/28.
//

#include "SceneSerializer.h"

#include "log/Logger.h"

string SceneSerializer::Save( Scene *scene) {
    Document document;
    document.SetObject();

    Serializer serializer;
    serializer.serialize(document, "scene", *scene);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

     Logger::error(buffer.GetString());
    return buffer.GetString();
//    std::cout << buffer.GetString() << std::endl;


}


template<>
void Serializer::serialize(Value &value, const char *name, const GameObject &obj) {
    Value vet(kObjectType);
    Serializer serializer;
    serializer.serialize(vet, "name", obj.getName());
    value.AddMember(StringRef(name),vet,allocator);

}


template<>
void Serializer::serialize(Value &value, const char *name, const std::vector<GameObject*> &v) {
    Value vet(kArrayType);
    Serializer serializer;
    for(const auto&go:v){
        Value obj(kObjectType);
        serializer.serialize(obj, *go);
        vet.PushBack(obj, allocator);
    }

    value.AddMember(StringRef(name),vet,allocator);

}
template<>
void Serializer::serialize(Value &value, const char *name, const Scene &v) {
    Value sc(kObjectType);
    Serializer serializer;

    serializer.serialize(sc, "name", v.name);
    serializer.serialize(sc,"gos",Scene::gameobjects);
    value.AddMember(StringRef(name), sc, allocator);
}

template<>
void Serializer::serialize(Value &value,const char *name,const Transform &obj){
    Value sc(kObjectType);
    Serializer serializer;
    serializer.serialize(sc,"p",obj.position);
    serializer.serialize(sc,"r",obj.rotate);
    serializer.serialize(sc,"s",obj.scale);

    value.AddMember(StringRef(name), sc, allocator);
}

template<>
void Serializer::serialize(Value &value,const GameObject &obj){
    Serializer serializer;
    serializer.serialize(value, "name", obj.getName());
//    serializer.serialize(value, "transform", obj.transform);

    const auto &comps=obj.getComponents();
    for(const auto&comp:comps){
        Value v=comp->serialize();
        value.AddMember(StringRef(comp->GetType()),v,allocator);
    }

}