//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_COMMANDNODE_H
#define OPENGLPROJ_COMMANDNODE_H


#include "ui/SceneNode.h"

class CommandNode: public SceneNode{
public:
    std::function<void(const Context&ctx)> DrawCommand;
public:
    CommandNode();
protected:
    void UpdateShader(const Context &ctx) override;
};


#endif //OPENGLPROJ_COMMANDNODE_H
