//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_SCENESERIALIZER_H
#define OPENGLPROJ_SCENESERIALIZER_H

#include <data/Serializer.h>
#include "core/Scene.h"
#include "core/GameObject.h"
class SceneSerializer:public Serializer{
public:
    string Save(class Scene*scene);
};

template<>
void Serializer::serialize(Value &value, const char *name, const GameObject &obj);

template<>
void Serializer::serialize(Value &value,const GameObject &obj);

template<>
void Serializer::serialize(Value &value, const char *name, const std::vector<GameObject*> &v);
template<>
void Serializer::serialize(Value &value, const char *name, const Scene &v);

template<>
void Serializer::serialize(Value &value,const char *name,const Transform &obj);

#endif //OPENGLPROJ_SCENESERIALIZER_H
