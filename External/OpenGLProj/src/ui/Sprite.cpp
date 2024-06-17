//
// Created by zhengshulin on 2024/5/31.
//

#include "Sprite.h"
#include "../core/resource_manager.h"


void Sprite::InitWithTexture(std::string path) {
    texture2D= (ResourceManager::LoadTexture(path.c_str(),path));
}

void Sprite::Draw() {
    renderer->DrawSprite(texture2D,position,size,rotate,color);
}

bool Sprite::Init(std::string path) {

    this->InitWithTexture(path);
    this->position=glm::vec2(200,100);
    this->size=glm::vec2(this->texture2D.width,this->texture2D.height);
    this->rotate=0;
    this->color=glm::vec3(1,1,1);
    this->renderer= new SpriteRenderer(ResourceManager::GetShader("sprite"));
    return false;
}


Sprite *Sprite::Create(std::string path) {
    auto sp=new Sprite();
    sp->Init(path);
    return sp;
}

void Sprite::Position(int x, int y) {
    this->position.x=x;
    this->position.y=y;

}

