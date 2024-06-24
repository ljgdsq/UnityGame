//
// Created by zhengshulin on 2024/6/20.
//

#include "Cube.h"
#include "CubeDrawable.h"
#include "core/ResourceManager.h"

Cube *Cube::Create(std::string path) {
    auto cube=new Cube();
    cube->Init(path);
    return cube;
}

bool Cube::Init(std::string path) {
    this->position=glm::vec3(0,0,0);
    this->size=glm::vec3(1,1,1);
    this->rotate=glm::vec3 (0,0,0);
    this->scale=glm::vec3(1,1,1);
    this->color=glm::vec3(1.0f,1.f,1.f);
    this->SetTexture(path);
    this->drawable=new CubeDrawable(ResourceManager::GetShader("cube"));
    return true;
}


void Cube::SetTexture(std::string path) {
    if (path.size()>0){
        this->path=path;
        hasTexture= true;
        texture2D= (ResourceManager::LoadTexture(path));
    }
}

void Cube::UpdateShader(const Context &ctx) {
    this->drawable->shader->Use();
    glm::mat4 model(1);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotate.z, glm::vec3(1.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model,size*scale);
    this->drawable->shader->SetVector3f("color",color);

    this->drawable->shader->SetMatrix4("model", model);
    this->drawable->shader->SetMatrix4("view", ctx.view);
    this->drawable->shader->SetMatrix4("projection", ctx.projection);
    if (hasTexture)
    {
        texture2D->Bind();
        this->drawable->shader->SetInteger("hasTex",1);
    }else{
        this->drawable->shader->SetInteger("hasTex",0);
    }
}

std::string Cube::GetTypeName() {
    return "Cube";
}

