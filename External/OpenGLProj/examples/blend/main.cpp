
#include "core/Application.h"
#include "core/buffer.h"

#include "ui/scene.h"
#include "ui/UISprite.h"
#include "3d/Cube.h"
#include "core/ResourceManager.h"
#include "3d/Plane.h"


class AdvanceScene : public Scene {

    Cube *cube;
public:

    void OnCreate() override {
    //blending_transparent_window.

        auto plane=Plane::Create("metal");
        this->AddChild(plane);
         plane->scale=glm::vec3 (10,10.f,10);
        plane->position=glm::vec3 (-0.5f,-0.501f,0.5f);
        plane->rotate=glm::vec3 (glm::radians(90.0f),0,0);

        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
        vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
        vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
        vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

        for (int i = 0; i < vegetation.size(); ++i) {
            auto grass=Plane::Create("grass");
            this->AddChild(grass);
            grass->position=vegetation[i];
            grass->texture2D.SetWrapMode(TextureParam::CLAMP_TO_EDGE);
            if (i%2==0){
                auto cube=Cube::Create("metal");
                this->AddChild(cube);
                cube->position=vegetation[i] +glm::vec3 (0.5f,0.5f,-0.501f);
            }

        }
    }


    void OnUpdate() override {


    }
};


int main() {

    Application application(800, 600, "advance");
    application.SetScene(new AdvanceScene());
    return application.Run();
}
