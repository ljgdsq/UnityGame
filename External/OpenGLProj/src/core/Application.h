//
// Created by zhengshulin on 2024/6/17.
//

#ifndef OPENGLPROJ_APPLICATION_H
#define OPENGLPROJ_APPLICATION_H


#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"

class Application {
    Application();
    bool valid;
    GLFWwindow* window;
    Renderer* renderer;
    Camera* camera;
public:
    virtual ~Application();
    void Init();
    void Run();
    static Application*GetInstance();

    GLFWwindow* GetWindow ()const{
        return window;
    }

    Renderer*GetRenderer() const{
        return renderer;
    }

    Camera *GetCamera() const{
        return camera;
    };

    bool ShouldClose();
    void Destroy();
};


#endif //OPENGLPROJ_APPLICATION_H
