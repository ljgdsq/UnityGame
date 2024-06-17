#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "core/Application.h"
#include "core/buffer.h"
#include "cube_vertex.h"
#include "core/shader.h"
#include "core/graphics.h"
#include "camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main() {
    auto app=Application::GetInstance();
    app->Init();



    VertexArray cubeVAO(VAO);
    Buffer VBO(VBO);

    VBO.uploadData(sizeof (cubeVertices),cubeVertices,DataUsage::STATIC_DRAW);
    cubeVAO.addBufferLayout({0,3,GL_FLOAT,GL_FALSE,5* sizeof(float ),0});
    cubeVAO.enableAttribute();
    VBO.unbind();
    cubeVAO.unbind();

    VertexArray lightCubeVAO(VAO);
    VBO.bind();
    lightCubeVAO.addBufferLayout({0,3,GL_FLOAT,GL_FALSE,5* sizeof(float ),0});
    lightCubeVAO.enableAttribute();

    Shader lightingShader("1.colors.vs", "1.colors.fs");
    Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    auto renderer=app->GetRenderer();
    renderer->SetClearMode(ClearMode::COLOR_BIT|ClearMode::DEPTH_BIT);
    renderer->Enable(FuncType::Depth_Test);
    Camera*camera=app->GetCamera();
    while (!app->ShouldClose())
    {
        renderer->Clear();
        renderer->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);


        lightingShader.Use();
        lightingShader.SetVector3f("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.SetVector3f("lightColor",  1.0f, 1.0f, 1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        lightingShader.SetMatrix4("projection", projection);
        lightingShader.SetMatrix4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.SetMatrix4("model", model);


          cubeVAO.bind();
          glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // also draw the lamp object
        lightCubeShader.Use();
        lightCubeShader.SetMatrix4("projection", projection);
        lightCubeShader.SetMatrix4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.SetMatrix4("model", model);
        lightCubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);


        renderer->Present();
        app->Run();
    }
    app->Destroy();


    return 0;
}
