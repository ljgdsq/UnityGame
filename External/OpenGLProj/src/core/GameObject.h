//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_GAMEOBJECT_H
#define OPENGLPROJ_GAMEOBJECT_H
#include <string>
#include <vector>
#include "memory"
#include "Component.h"
#include "Transform.h"
#include "core/component/ImageComponent.h"

using std::string;
using std::vector;
using std::unique_ptr;
class GameObject {
private:
    string name;
public:
    const string getName() const;

private:
    bool active;
    vector<Component*> components;
public:
    const vector<Component *> &getComponents() const;

public:
    explicit GameObject(const string &name);
    void Update(const Context&ctx);
public:
    void AddComponent(Component*component);
    void RemoveComponent(Component*component);
    Transform transform;

    template<class T>
    T *AddComponent();

};

template<>
ImageComponent* GameObject::AddComponent();




#endif //OPENGLPROJ_GAMEOBJECT_H
