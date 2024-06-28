

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "editor/EditorApplication.h"
#include <GLFW/glfw3.h>
#include "src/core/Scene.h"
#include "ui/UISprite.h"
#include "3d/Cube.h"
#include "editor/Hierarchy.h"
#include "core/GameObject.h"
#include "core/component/MeshComponent.h"
#include "core/bsp/CubeMesh.h"

class ImguiScene:public Scene{

    UISprite* sp2;

    Cube* cube;
    Cube* plane;
public:
    explicit ImguiScene(const string &name) : Scene(name) {}

    void OnCreate() override {
//        sp=UISprite::Create("face");
//        sp->Scale(0.5f,0.5f);
//        this->AddChild(sp);
//
//    auto sp1=UISprite::Create("container");
//    sp1->Position(500,200);
//    this->AddChild(sp1);

        {
            auto cube=new GameObject("Cube");
            auto comp=cube->AddComponent<MeshComponent>();
            cube->transform.position=vec3 (0,0,0);
            comp->SetUpMesh(new CubeMesh());
        }


        {
            auto go=new GameObject("zzz");
            auto comp=go->AddComponent<ImageComponent>();
            comp->setTexture2D(ResourceManager::GetTexture("face"));
            go->transform.position=vec3 (100,100,0);

        }
        {        int batch=2;
            for (int i = 0; i <batch ; ++i) {
                auto go=new GameObject("zzz"+ std::to_string(i));
                auto comp=go->AddComponent<ImageComponent>();
                comp->setTexture2D(ResourceManager::GetTexture("face"));
                go->transform.scale=vec3 (0.5f,0.5f,1);
                go->transform.position=vec3 (100+rand()%500,100+rand()%500,0);
            }

        }




//
//        sp2=UISprite::Create("container");
//        sp2->Position(512,512);
//        this->AddChild(sp2);
//        auto w=Application::GetInstance()->GetWidth();
//        auto h=Application::GetInstance()->GetHeight();
//
//        auto sp2=UISprite::Create("container");
//        sp2->Position(w/2,h/2);
//        this->AddChild(sp2);
////
//        cube=Cube::Create("wall");
//        this->AddChild(cube);
//        cube->scale=glm::vec3 (0.5,0.5,0.5);


    }


};


int main() {
    EditorApplication app(1920, 1080, "editor");
    app.SetScene(new ImguiScene("imgui"));
    return app.Run();
}

