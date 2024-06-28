//
// Created by zhengshulin on 2024/6/27.
//

#include "GameObject.h"
#include "Scene.h"
#include "core/component/MeshComponent.h"

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

const string GameObject::getName() const {
    return name;
}

void GameObject::Update(const Context &ctx) {
    for(const auto&comp:components){
        comp->OnUpdate(ctx);
    }
}

const vector<Component *> &GameObject::getComponents() const {
    return components;
}


template<>
ImageComponent* GameObject::AddComponent(){
    const auto&comp=new ImageComponent(this);
    comp->OnCreate();
    this->AddComponent(comp);
    return  comp;
}


template<>
MeshComponent* GameObject::AddComponent(){
    const auto&comp=new MeshComponent(this);
    comp->OnCreate();
    this->AddComponent(comp);
    return  comp;
}