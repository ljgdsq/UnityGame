//
// Created by zhengshulin on 2024/6/12.
//

#include "graphics.h"

Graphics *Graphics::GetInstance() {
    static Graphics graphics;
    return &graphics;
}

void Graphics::SetDrawMode(DrawMode mode) {
    glPolygonMode(GL_FRONT_AND_BACK,(int)mode);
}
