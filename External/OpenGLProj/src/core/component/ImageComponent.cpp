//
// Created by zhengshulin on 2024/6/27.
//

#include "ImageComponent.h"
#include "core/GameObject.h"
std::string ImageComponent::GetType() {
    return "Image";
}

void ImageComponent::OnCreate() {
    Canvas::AddCanvasItem(this);
}

void ImageComponent::OnEnable() {

}

void ImageComponent::OnUpdate() {
    Component::OnUpdate();
}

void ImageComponent::OnDisable() {
}

void ImageComponent::OnDestroy() {
    Canvas::RemoveCanvasItem(this);
}

Texture2D *ImageComponent::getTexture2D() const {
    return texture2D;
}

void ImageComponent::setTexture2D(Texture2D *texture2D) {
    ImageComponent::texture2D = texture2D;
}

void ImageComponent::Process() {
    QuadDrawCommand quadDrawCommand;
    quadDrawCommand.scale=gameObject->transform.scale;
    quadDrawCommand.size=vec2 (texture2D->width,texture2D->height);
    Canvas::SubmitDrawCommand(quadDrawCommand);
}

ImageComponent::ImageComponent(GameObject *gameObject) : CanvasItem(gameObject) {}

