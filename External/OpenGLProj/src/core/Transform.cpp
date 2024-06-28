//
// Created by zhengshulin on 2024/6/27.
//

#include "Transform.h"

std::string Transform::GetType() {
    return "Transform";
}

void Transform::OnCreate() {

}

void Transform::OnEnable() {

}

void Transform::OnDisable() {

}

void Transform::OnDestroy() {

}

Transform::Transform(GameObject *gameObject, const vec3 &position, const vec3 &scale, const vec3 &rotate) : Component(
        gameObject), position(position), scale(scale), rotate(rotate) {}

Transform::Transform(GameObject *gameObject) : Component(gameObject), position(0,0,0),scale(1,1,1), rotate(0) {}
