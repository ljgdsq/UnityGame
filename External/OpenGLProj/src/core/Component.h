//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_COMPONENT_H
#define OPENGLPROJ_COMPONENT_H
#include "Common.h"
class GameObject;
class Component {


public:
    explicit Component(GameObject *gameObject) : gameObject(gameObject) {}
    bool active= true;
    GameObject*gameObject;
protected:
    virtual std::string GetType()=0;
protected:
    virtual void OnCreate()=0;
    virtual void OnEnable()=0;
    virtual void OnUpdate(){};
    virtual void OnDisable()=0;
    virtual void OnDestroy()=0;

};


#endif //OPENGLPROJ_COMPONENT_H
