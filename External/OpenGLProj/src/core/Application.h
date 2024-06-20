//
// Created by zhengshulin on 2024/6/17.
//

#ifndef OPENGLPROJ_APPLICATION_H
#define OPENGLPROJ_APPLICATION_H


#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include "Context.h"

class Application {

    bool valid;
    GLFWwindow* window;
    Renderer* renderer;
    Camera* camera;
    void Init();

public:
    Application(int width,int height,const char*title= nullptr);
    virtual ~Application();
    int Run();
    [[deprecated("old api,do not use")]]
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

    Context*GetContext()const;

    int GetWidth() const;

    int GetHeight() const;

    bool ShouldClose();
    void Destroy();
};


#endif //OPENGLPROJ_APPLICATION_H
