//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_MESHCOMPONENT_H
#define OPENGLPROJ_MESHCOMPONENT_H


#include "core/Component.h"
#include "core/texture.h"
#include "core/Canvas.h"
#include "core/shader.h"
class MeshComponent: public Component {
    const char* GetType() override;

public:
    rapidjson::Value serialize() override;

    void deserialize() override;

private:

    void OnEnable() override;

    void OnDisable() override;

    void OnDestroy() override;

    class Quad3DDrawCommand;
    Quad3DDrawCommand *quad3DDrawCommand;
    class Mesh*mesh;
public:
    explicit MeshComponent(GameObject *gameObject);
    Texture2D*texture2D= nullptr;
    Shader*shader= nullptr;

    void SetUpMesh(class Mesh*mesh);
    void OnCreate() override;

private:

public:
    void OnUpdate(const Context &ctx) override;

};


#endif //OPENGLPROJ_MESHCOMPONENT_H
