//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_COMPONENTINSPECTOR_H
#define OPENGLPROJ_COMPONENTINSPECTOR_H


class ComponentInspector {
public:
    virtual void Inspect(class GameObject*node)=0;
};


#endif //OPENGLPROJ_COMPONENTINSPECTOR_H
