//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_TRANSFORM_H
#define OPENGLPROJ_TRANSFORM_H

#include "Component.h"
class Transform: public Component{
public:
    rapidjson::Value serialize() override;

    void deserialize() override;

public:
    vec3 position;
    vec3 scale;
    vec3 rotate;

    Transform(GameObject *gameObject, const vec3 &position, const vec3 &scale, const vec3 &rotate);

    explicit Transform(GameObject *gameObject);

protected:
    const char* GetType() override;

    void OnCreate() override;

    void OnEnable() override;

    void OnDisable() override;

    void OnDestroy() override;

};


#endif //OPENGLPROJ_TRANSFORM_H
