#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "core/Application.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/graphics.h"
#include "camera.h"
#include "data/code_data.h"
#include "core/ResourceManager.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main() {
    auto app=Application::GetInstance();
     ;

    VertexArray cubeVAO(VAO);
    Buffer VBO(VBO);

    VBO.uploadData(sizeof (vertices),vertices,DataUsage::STATIC_DRAW);
    cubeVAO.addBufferLayout({0,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),0});
    cubeVAO.addBufferLayout({1,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),3* sizeof(float )});
    cubeVAO.addBufferLayout({2,2,GL_FLOAT,GL_FALSE,8* sizeof(float ),6* sizeof(float )});
    cubeVAO.enableAttribute(0);
    cubeVAO.enableAttribute(1);
    cubeVAO.enableAttribute(2);
    VBO.unbind();
    cubeVAO.unbind();

    VertexArray lightCubeVAO(VAO);
    VBO.bind();
    lightCubeVAO.addBufferLayout({0,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),0});
    lightCubeVAO.enableAttribute();

    Shader lightingShader("4.1.lighting_maps.vs", "4.1.lighting_maps.fs");
    Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    auto renderer=app->GetRenderer();
    renderer->SetClearMode(ClearMode::COLOR_BIT|ClearMode::DEPTH_BIT);
    renderer->Enable(FuncType::Depth_Test);
    Camera*camera=app->GetCamera();



    Texture2D tex=ResourceManager::LoadTexture("container2","container2");
    Texture2D tex2=ResourceManager::LoadTexture("container2_specular","container2_specular");

    lightingshader->Use();
    lightingshader->SetInteger("material.diffuse", 0);
    lightingshader->SetInteger("material.specular", 1);
    // light properties
    lightingshader->SetVector3f("light.ambient", 0.2f, 0.2f, 0.2f);
    lightingshader->SetVector3f("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingshader->SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
//    lightingshader->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.SetFloat("material.shininess", 64.0f);



    while (!app->ShouldClose())
    {
        renderer->Clear();
        renderer->ClearColor(0.1f, 0.1f, 0.1f, 1.0f);


        lightingshader->Use();
        lightingshader->SetVector3f("light.position", lightPos);
        lightingshader->SetVector3f("viewPos", camera->Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        lightingshader->SetMatrix4("projection", projection);
        lightingshader->SetMatrix4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingshader->SetMatrix4("model", model);
        tex.Bind(0);

        tex2.Bind(1);
        cubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightCubeshader->Use();
        lightCubeshader->SetMatrix4("projection", projection);
        lightCubeshader->SetMatrix4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeshader->SetMatrix4("model", model);
        lightCubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        renderer->Present();
        app->Run();
    }
    app->Destroy();


    return 0;
}
