//
// Created by zhengshulin on 2024/6/20.
//

#include "Scene.h"



void Scene::Update() {
    for(const auto node:nodes){
        node->Update();
    }
    OnUpdate();
}

void Scene::Create() {
    OnCreate();
}

void Scene::Render(const Context&ctx) {
    for(const auto node:nodes){
        node->Draw(ctx);
    }
}

void Scene::AddChild(SceneNode *child) {
    nodes.push_back(child);
}

void Scene::DeleteChild(int index) {
    if (index>=0&&index<nodes.size()){
        nodes.erase(nodes.begin()+index);
    }
}
