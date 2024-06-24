
#include "core/Application.h"
#include "core/buffer.h"

#include "ui/scene.h"
#include "ui/UISprite.h"
#include "3d/Cube.h"
#include "core/ResourceManager.h"


class AdvanceScene:public Scene{

    UISprite* sp;
    UISprite* sp2;

    Cube* cube;
    Cube* cube2;
    Cube* plane;
public:

    void OnCreate() override {
//        sp=UISprite::Create("face");
//        sp->Scale(0.5f,0.5f);
//        this->AddChild(sp);
//
//        sp2=UISprite::Create("container");
//        sp2->Position(-10,200);
//        this->AddChild(sp2);
//
        cube=Cube::Create("wall");
//        this->AddChild(cube);
        cube->scale=glm::vec3 (0.5,0.5,0.5);

//        cube2=Cube::Create("wall");
//        this->AddChild(cube2);
//        cube2->position=glm::vec2 (1,2);

        plane=Cube::Create("metal");
//        this->AddChild(plane);
        plane->position=glm::vec2 (0,0);
        plane->scale=glm::vec3(10,0.0001,10);
        context->renderer->SetClearMode(ClearMode::DEPTH_BIT|ClearMode::COLOR_BIT|ClearMode::STENCIL_BIT);

//        cube3=Cube::Create();
//        this->AddChild(cube3);

//        xyzshader=ResourceManager::GetShader("xyz");
//        VAO=new VertexArray();
//        VBO=new Buffer(BufferType::VBO);
//        float length=999.0;
//        float vertices[] = {
//                // X axis (red)
//                -length, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
//                length, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
//                // Y axis (green)
//                0.0f, -length, 0.0f,  0.0f, 1.0f, 0.0f,
//                0.0f, length, 0.0f,  0.0f, 1.0f, 0.0f,
//                // Z axis (blue)
//                0.0f, 0.0f, -length,  0.0f, 0.0f, 1.0f,
//                0.0f, 0.0f, length,  0.0f, 0.0f, 1.0f
//        };
//
//        VBO->uploadData(sizeof(vertices),vertices,DataUsage::STATIC_DRAW);
//        VAO->addBufferLayout({0,3,GL_FLOAT, false,6* sizeof(float ),0});
//        VAO->addBufferLayout({1,3,GL_FLOAT, false,6* sizeof(float ),3* sizeof(float )});
//        VAO->enableAttribute(0);
//        VAO->enableAttribute(1);
//        VBO->unbind();
//        VAO->unbind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    VertexArray*VAO;
    Buffer*VBO;
    float t=0;
    Shader xyzshader;
    void OnUpdate() override {
//        xyzshader->Use();
//        glm::mat4 model = glm::mat4(1.0f);
//
//
//        // Set the uniforms
//        glUniformMatrix4fv(glGetUniformLocation(xyzshader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
//        glUniformMatrix4fv(glGetUniformLocation(xyzshader.id, "view"), 1, GL_FALSE, glm::value_ptr(context->view));
//        glUniformMatrix4fv(glGetUniformLocation(xyzshader.id, "projection"), 1, GL_FALSE, glm::value_ptr(context->projection));
//
//        // Bind VAO and draw the axes
//        glBindVertexArray(VAO->getId());
//        glDrawArrays(GL_LINES, 0, 6);
//        glBindVertexArray(0);

        context->renderer->Clear();
        context->renderer->SetClearMode(ClearMode::DEPTH_BIT|ClearMode::COLOR_BIT|ClearMode::STENCIL_BIT);
        glStencilMask(0x00);
        plane->Draw(*context);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        cube->scale=glm::vec3 (1.0f,1.0f,1.0f);
        cube->color=glm::vec3 (1.0f,1.0f,1.0f);
        cube->Draw(*context);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
        float  sacle=1.08f;
        cube->scale=glm::vec3 (sacle,sacle,sacle);
        cube->color=glm::vec3 (1.0f,0.1f,0.1f);
        cube->Draw(*context);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);



    }
};


int main() {

    Application application(800,600,"advance");
    application.SetScene(new AdvanceScene());
    return application.Run();
}
