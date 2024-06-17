//
// Created by zhengshulin on 2024/5/31.
//

#ifndef OPENGLPROJ_SPRITE_H
#define OPENGLPROJ_SPRITE_H

#include "../core/sprite_renderer.h"
#include "../core/Drawable.h"

class Sprite : Drawable {
public:
    static Sprite*Create(std::string path);
    Sprite()=default;
    bool Init(std::string path);
    void Draw() override;
    void Position(int x,int y);
private:
    void InitWithTexture(std::string path);

    SpriteRenderer *renderer;
    Texture2D texture2D;
    glm::vec2 position;
    glm::vec2 size;
    float rotate;
    glm::vec3 color;
};


#endif //OPENGLPROJ_SPRITE_H
