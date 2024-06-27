//
// Created by zhengshulin on 2024/6/27.
//

#include "GameObject.h"
#include "ui/Scene.h"
GameObject::GameObject(const string &name) : name(name),active(true), transform(this) {
    Scene::gameobjects.push_back(this);

}

void GameObject::AddComponent(Component *component) {
    components.push_back(component);
}

void GameObject::RemoveComponent(Component *component) {
    auto it=std::remove(components.begin(),components.end(),component);
    if (it != components.end()) {
        components.erase(it, components.end());
    }
}


template<>
ImageComponent* GameObject::AddComponent(){
    return  new ImageComponent(this);
}