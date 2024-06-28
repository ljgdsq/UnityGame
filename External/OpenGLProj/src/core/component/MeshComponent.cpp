//
// Created by zhengshulin on 2024/6/28.
//

#include "MeshComponent.h"
#include "core/ResourceManager.h"
#include "core/Mesh.h"
#include "core/MeshRenderer.h"
#include "core/GameObject.h"
MeshComponent::MeshComponent(GameObject *gameObject) : Component(gameObject) {}

void MeshComponent::OnCreate() {

}
void MeshComponent::OnUpdate(const Context &ctx){
    if (mesh &&mesh->shader){
        mesh->shader->Use();

        glm::mat4 model(1);
        model = glm::translate(model, gameObject->transform.position);
        model = glm::rotate(model, gameObject->transform.rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, gameObject->transform.rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, gameObject->transform.rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model,gameObject->transform.scale);

        shader->SetMatrix4("model", model);
        shader->SetMatrix4("view", ctx.view);
        shader->SetMatrix4("projection", ctx.projection);

    }
}

void MeshComponent::SetUpMesh( Mesh *mesh) {
    this->mesh=mesh;
    if (mesh->shader){
        this->shader=mesh->shader;
    }
    MeshRenderer::SubmitMesh(mesh);
}

const char* MeshComponent::GetType() {
    return "MeshComponent";
}

void MeshComponent::OnEnable() {

}

void MeshComponent::OnDisable() {

}

void MeshComponent::OnDestroy() {

}

rapidjson::Value MeshComponent::serialize() {
    return rapidjson::Value();
}

void MeshComponent::deserialize() {

}


