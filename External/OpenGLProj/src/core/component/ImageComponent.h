//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_IMAGECOMPONENT_H
#define OPENGLPROJ_IMAGECOMPONENT_H

#include "core/Component.h"
#include "core/texture.h"
#include "core/Canvas.h"

class ImageComponent:CanvasItem {
public:
    explicit ImageComponent(GameObject *gameObject);

private:
    Texture2D*texture2D= nullptr;
public:
    Texture2D *getTexture2D() const;

    void setTexture2D(Texture2D *texture2D);

private:
    void Process() override;

protected:
    std::string GetType() override;

    void OnCreate() override;

    void OnEnable() override;

    void OnUpdate() override;

    void OnDisable() override;

    void OnDestroy() override;

};


#endif //OPENGLPROJ_IMAGECOMPONENT_H
