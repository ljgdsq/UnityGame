
#include "core/Application.h"
#include "core/buffer.h"

#include "ui/scene.h"
#include "ui/UISprite.h"
#include "3d/Cube.h"


class AdvanceScene:public Scene{

    UISprite* sp;
    UISprite* sp2;

    Cube* cube;
public:

    void OnCreate() override {
        sp=UISprite::Create("face");
        sp->Scale(0.5f,0.5f);
        this->AddChild(sp);

        sp2=UISprite::Create("container");
        sp2->Position(-10,200);
        this->AddChild(sp2);

        cube=Cube::Create();
        this->AddChild(cube);


    }

    float t=0;
    void OnUpdate() override {
        t++;
//        sp->Angle(t);
    }
};


int main() {

    Application application(800,600,"advance");
    application.SetScene(new AdvanceScene());
    return application.Run();
}
