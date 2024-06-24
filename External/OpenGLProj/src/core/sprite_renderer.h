//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_SPRITE_RENDERER_H
#define OPENGLPROJ_SPRITE_RENDERER_H


#include "shader.h"
#include "texture.h"
#if 0
class SpriteRenderer
{
public:
    SpriteRenderer(const Shader &shader);
    ~SpriteRenderer();

    void DrawSprite(Texture2D &texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    int quadVAO;

    void initRenderData();
};
#endif
#endif //OPENGLPROJ_SPRITE_RENDERER_H
