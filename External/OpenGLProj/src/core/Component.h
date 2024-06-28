//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_COMPONENT_H
#define OPENGLPROJ_COMPONENT_H
#include "Common.h"
#include "Context.h"
#include "core/component/Serializable.h"

class GameObject;
class Component:public Serializable {


public:
    explicit Component(GameObject *gameObject) : gameObject(gameObject) {}
    bool active= true;
    GameObject*gameObject;

    virtual void OnUpdate(const Context&ctx){}

    virtual const char* GetType()=0;

protected:
    virtual void OnCreate()=0;
    virtual void OnEnable()=0;;
    virtual void OnDisable()=0;
    virtual void OnDestroy()=0;

};


#endif //OPENGLPROJ_COMPONENT_H
