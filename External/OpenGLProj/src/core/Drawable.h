//
// Created by zhengshulin on 2024/5/31.
//

#ifndef OPENGLPROJ_DRAWABLE_H
#define OPENGLPROJ_DRAWABLE_H
#include "core/shader.h"
#include "buffer.h"
#include <functional>
#include "core/Context.h"
class Drawable{
private:
//    std::function<void(const Context&ctx ) > ApplyState;
public:
    Shader shader;
    Drawable(){};//TODO REMOVE THIS shade be a pointer
    Drawable(const Shader&shader):shader(shader){};
//    void SetUpdateStateFunc(std::function<void(const Context&ctx)> func){
//        ApplyState=func;
//    }
    virtual ~Drawable(){}
//    void  ApplyStateChange(const Context&ctx){
//        if (ApplyState)
//            ApplyState(ctx);
//    };
public:
    virtual void Draw()=0;
};



#endif //OPENGLPROJ_DRAWABLE_H
