//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_CONTEXT_H
#define OPENGLPROJ_CONTEXT_H
#include <string>

class Context {

public:
//    static Context*GetInstance();
    int width;
    int height;
    std::string title;
    void*window;
};


#endif //OPENGLPROJ_CONTEXT_H
