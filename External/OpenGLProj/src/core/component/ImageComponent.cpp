//
// Created by zhengshulin on 2024/6/27.
//

#include "ImageComponent.h"
#include "core/GameObject.h"
#include "core/ResourceManager.h"

std::string ImageComponent::GetType() {
    return "Image";
}

void ImageComponent::OnCreate() {
    Canvas::AddCanvasItem(this);
    shader=ResourceManager::GetShader("sprite2");
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
    this->texture2D = texture2D;
}

void ImageComponent::Process(const Context&ctx) {
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f, (float) ctx.width,
                                      (float) ctx.height, 0.0f, -1.0f, 1.0f);
    shader->SetMatrix4("projection", projection);

    quadDrawCommand.position = gameObject->transform.position;
    quadDrawCommand.scale = gameObject->transform.scale;
    if (texture2D != nullptr) {
        quadDrawCommand.texID = texture2D->id;
        quadDrawCommand.size = vec2(texture2D->width, texture2D->height);
        quadDrawCommand.shaderID=shader->id;
    } else {
        quadDrawCommand.shaderID = -1;
        quadDrawCommand.texID = -1;
        quadDrawCommand.size = vec2 (100);
    }
    quadDrawCommand.rotate = gameObject->transform.rotate;
    texture2D->Bind();
    Canvas::SubmitDrawCommand(&quadDrawCommand);
}

ImageComponent::ImageComponent(GameObject *gameObject) : CanvasItem(gameObject), quadDrawCommand() {}

