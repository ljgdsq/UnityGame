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
    this->position=glm::vec2(200,100);
    this->size=glm::vec2(this->texture2D.width,this->texture2D.height);
    this->rotate=0;
    this->scale=glm::vec2(1,1);
    this->color=glm::vec3(1,1,1);

    this->drawable=new QuadDrawable(ResourceManager::GetShader("sprite"));
    this->drawable->SetUpdateStateFunc([=](const Context&ctx){
        this->UpdateShader();
    });
    UpdateShader();
    return false;
}
void UISprite::InitWithTexture(std::string path) {
    texture2D= (ResourceManager::LoadTexture(path.c_str(),path));
}

void UISprite::Position(int x, int y) {
    this->position.x=x;
    this->position.y=y;
}

void UISprite::UpdateShader() {
    this->drawable->shader.Use();
    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size*scale, 1.0f));
    this->drawable->shader.SetMatrix4("model", model);
    this->drawable->shader.SetVector3f("spriteColor", color);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800),
                                      static_cast<GLfloat>(600), 0.0f, -1.0f, 1.0f);
    this->drawable->shader.SetMatrix4("projection", projection);
    texture2D.Bind();

}

void UISprite::Scale(float x, float y) {
    this->scale.x=x;
    this->scale.y=y;
}

void UISprite::Angle(float angle) {
    this->rotate=glm::radians(angle);
}
