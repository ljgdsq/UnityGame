//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_CONTEXT_H
#define OPENGLPROJ_CONTEXT_H
#include <string>
#include "glm/glm.hpp"
#include "Renderer.h"

class Context {

public:

    int width;
    int height;
    std::string title;
    glm::mat4 projection;
    glm::mat4 view;
    void*window= nullptr;
    Renderer* renderer= nullptr;
    void*fbo= nullptr;
    bool ignoreInput = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};


#endif //OPENGLPROJ_CONTEXT_H
