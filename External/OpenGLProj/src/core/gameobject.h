//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_GAMEOBJECT_H
#define OPENGLPROJ_GAMEOBJECT_H


#include "texture.h"
#include "sprite_renderer.h"
#include <glm/glm.hpp>

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // Object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float     Rotation;
    bool   IsSolid;
    bool   Destroyed;
    // Render state
    Texture2D   Sprite;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif //OPENGLPROJ_GAMEOBJECT_H
