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

Sprite *Sprite::Create(std::string path) {
    auto sp=new Sprite();
    sp->InitWithTexture(path);
    sp->position=glm::vec2(200,100);
    sp->size=glm::vec2(sp->texture2D.width,sp->texture2D.height);
    sp->rotate=0;
    sp->color=glm::vec3(1,1,1);
    sp->renderer= new SpriteRenderer(ResourceManager::GetShader("sprite"));
    return sp;
}

void Sprite::Position(int x, int y) {
    this->position.x=x;
    this->position.y=y;

}
