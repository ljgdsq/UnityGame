//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_IMAGECOMPONENT_H
#define OPENGLPROJ_IMAGECOMPONENT_H

#include "core/Component.h"
#include "core/texture.h"
#include "core/Canvas.h"
#include "core/shader.h"

class ImageComponent:CanvasItem {
public:
    explicit ImageComponent(GameObject *gameObject);

    void OnCreate() override;

private:
    Texture2D*texture2D= nullptr;
    Shader*shader= nullptr;
    QuadDrawCommand quadDrawCommand;
public:
    Texture2D *getTexture2D() const;

    void setTexture2D(Texture2D *texture2D);

private:
    void Process(const Context&ctx) override;

protected:
    std::string GetType() override;

    void OnEnable() override;

    void OnUpdate() override;

    void OnDisable() override;

    void OnDestroy() override;


};


#endif //OPENGLPROJ_IMAGECOMPONENT_H
