

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "editor/EditorApplication.h"
#include <GLFW/glfw3.h>
#include "ui/scene.h"
#include "ui/UISprite.h"
#include "3d/Cube.h"
#include "editor/Hierarchy.h"

class ImguiScene:public Scene{

    UISprite* sp2;

    Cube* cube;
    Cube* plane;
public:

    void OnCreate() override {
//        sp=UISprite::Create("face");
//        sp->Scale(0.5f,0.5f);
//        this->AddChild(sp);
//
        sp2=UISprite::Create("container");
        sp2->Position(-10,200);
        this->AddChild(sp2);
//
        cube=Cube::Create("wall");
      this->AddChild(cube);
        cube->scale=glm::vec3 (0.5,0.5,0.5);


    }


};


int main() {
    EditorApplication app(1920, 1080, "editor");
    app.SetScene(new ImguiScene());
    return app.Run();
}