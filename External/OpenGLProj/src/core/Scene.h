//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_SCENE_H
#define OPENGLPROJ_SCENE_H
#include <vector>
#include "ui/SceneNode.h"
#include "core/Context.h"
#include "camera.h"

class Scene {
    friend class Application;
    friend class GameObject;
    friend class EditorApplication;
    friend class Serializer;
    friend class SceneSerializer;
    void Update(const Context&ctx);

public:
    explicit Scene(const string &name);

private:
    void Create();
protected:
    std::vector<SceneNode*> nodes;
    static std::vector<class GameObject*> gameobjects;
    Context*context;
    class Canvas*canvas;
    class MeshRenderer*meshRenderer;
    virtual void Render(const Context&ctx);
public:
    string name;
    virtual void OnCreate(){};
    virtual void OnUpdate(){

    };
    void SetContext(Context*ctx){
        this->context=ctx;
    }
    void AddChild(SceneNode*child);
    void DeleteChild(int index);
    virtual void Destroy(){};

   virtual std::vector<SceneNode*> GetNodes(){
        return nodes;
    }


};


#endif //OPENGLPROJ_SCENE_H
