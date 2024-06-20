//
// Created by zhengshulin on 2024/6/20.
//

#include "Cube.h"
#include "CubeDrawable.h"
#include "core/resource_manager.h"

Cube *Cube::Create(std::string path) {
    auto cube=new Cube();
    cube->Init(path);
    return cube;
}

bool Cube::Init(std::string path) {
    this->position=glm::vec2(0,0);
    this->size=glm::vec2(1,1);
    this->rotate=0;
    this->scale=glm::vec2(1,1);
    this->color=glm::vec3(0.4f,0.4f,0.4f);
    this->SetTexture(path);
    this->drawable=new CubeDrawable(ResourceManager::GetShader("cube"));
    this->drawable->SetUpdateStateFunc([=](const Context&ctx){
        this->UpdateShader(ctx);
    });
    return true;
}

void Cube::UpdateShader(const Context&ctx) {
    this->drawable->shader.Use();
    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size*scale, 1.0f));
    this->drawable->shader.SetVector3f("color",color);
    this->drawable->shader.SetMatrix4("model", model);
    this->drawable->shader.SetMatrix4("view", ctx.view);
    this->drawable->shader.SetMatrix4("projection", ctx.projection);
    if (hasTexture)
        texture2D.Bind();
}

void Cube::SetTexture(std::string path) {
    if (path.size()>0){
        this->path=path;
        hasTexture= true;
        texture2D= (ResourceManager::LoadTexture(path.c_str(),path));
    }
}

