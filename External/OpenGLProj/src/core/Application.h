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
class Scene;
class Application {

    bool valid;

public:
     static bool isIgnoreInput() ;

     static void setIgnoreInput(bool ignoreInput);

    static Context*context;
protected:
    virtual void Init();
    virtual void BeginFrame(){};
    virtual void EndFrame(){}
    virtual void OnExit(){}
    Renderer* renderer= nullptr;
    Camera* camera= nullptr;
    GLFWwindow* window= nullptr;
    Scene*scene= nullptr;
public:
    Application(int width,int height,const char*title= nullptr);
    virtual ~Application();
    int Run();
    virtual void SetScene(Scene*scene);
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
