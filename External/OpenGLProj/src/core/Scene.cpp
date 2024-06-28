//
// Created by zhengshulin on 2024/6/20.
//

#include "Scene.h"
#include "core/Canvas.h"
#include "core/MeshRenderer.h"
#include "core/GameObject.h"
std::vector<class GameObject*> Scene::gameobjects;

void Scene::Update(const Context&ctx) {
    for(const auto node:nodes){
        node->Update();
    }

    for(const auto &go:gameobjects){
        go->Update(ctx);
    }
    OnUpdate();
}

void Scene::Create() {
    canvas=new Canvas(0);
    canvas->OnCreate();

    meshRenderer=new MeshRenderer();
    OnCreate();
}

void Scene::Render(const Context&ctx) {

    for(const auto node:nodes){
        node->Draw(ctx);
    }
    meshRenderer->RenderAll(ctx);
    canvas->Process(ctx);
    canvas->Draw();
}

void Scene::AddChild(SceneNode *child) {
    nodes.push_back(child);
}

void Scene::DeleteChild(int index) {
    if (index>=0&&index<nodes.size()){
        nodes.erase(nodes.begin()+index);
    }
}

Scene::Scene(const string &name) : name(name) {}
