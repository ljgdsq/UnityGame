//
// Created by zhengshulin on 2024/6/20.
//

#include "UISprite.h"
#include "core/resource_manager.h"
#include "QuadDrawable.h"

UISprite *UISprite::Create(std::string path) {

    auto sp=new UISprite();
    sp->Init(path);
    return sp;
}

bool UISprite::Init(std::string path) {
    this->InitWithTexture(path);
    this->position=glm::vec3(200,100,0);
    this->size=glm::vec3(this->texture2D.width,this->texture2D.height,1);
    this->rotate=glm::vec3 (0,0,0);
    this->scale=glm::vec3(1,1,1);
    this->color=glm::vec3(1,1,1);
    this->drawable=new QuadDrawable(ResourceManager::GetShader("sprite"));
    return false;
}
void UISprite::InitWithTexture(std::string path) {
    texture2D= (ResourceManager::LoadTexture(path.c_str(),path));
}

void UISprite::Position(int x, int y) {
    this->position.x=x;
    this->position.y=y;
}

void UISprite::Scale(float x, float y) {
    this->scale.x=x;
    this->scale.y=y;
}

void UISprite::Angle(float angle) {
    this->rotate.z=glm::radians(angle);
}

void UISprite::UpdateShader(const Context &ctx) {
    this->drawable->shader.Use();
    glm::mat4 model(1);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotate.z, glm::vec3(1.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, size*scale);
    this->drawable->shader.SetMatrix4("model", model);
    this->drawable->shader.SetVector3f("spriteColor", color);
    glm::mat4 projection = glm::ortho(0.0f, (float)ctx.width,
                                      (float)ctx.height, 0.0f, -1.0f, 1.0f);
    this->drawable->shader.SetMatrix4("projection", projection);
    texture2D.Bind();
}

std::string UISprite::GetTypeName() {
    return "UISprite";
}
