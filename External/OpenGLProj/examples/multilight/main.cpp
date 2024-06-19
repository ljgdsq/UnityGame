
#include "core/Application.h"
#include "core/buffer.h"
#include "core/shader.h"
#include "core/graphics.h"
#include "camera.h"
#include "data/code_data.h"
#include "core/resource_manager.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main() {
    auto app=Application::GetInstance();
    app->Init();

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

    Shader lightingShader("5.1.multi_light.vs", "5.1.multi_light.fs");
    Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    auto renderer=app->GetRenderer();
    renderer->SetClearMode(ClearMode::COLOR_BIT|ClearMode::DEPTH_BIT);
    renderer->Enable(FuncType::Depth_Test);
    Camera*camera=app->GetCamera();



    Texture2D tex=ResourceManager::LoadTexture("container2","container2");
    Texture2D tex2=ResourceManager::LoadTexture("container2_specular","container2_specular");

    lightingShader.Use();
    lightingShader.SetInteger("material.diffuse", 0);
    lightingShader.SetInteger("material.specular", 1);

    // material properties
//    lightingShader.SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.SetFloat("material.shininess", 32.0f);

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // directional light
    lightingShader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lightingShader.SetVector3f("pointLights[0].position", pointLightPositions[0]);
    lightingShader.SetVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.SetVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.SetVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.SetFloat("pointLights[0].constant", 1.0f);
    lightingShader.SetFloat("pointLights[0].linear", 0.09f);
    lightingShader.SetFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    lightingShader.SetVector3f("pointLights[1].position", pointLightPositions[1]);
    lightingShader.SetVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.SetVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.SetVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.SetFloat("pointLights[1].constant", 1.0f);
    lightingShader.SetFloat("pointLights[1].linear", 0.09f);
    lightingShader.SetFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    lightingShader.SetVector3f("pointLights[2].position", pointLightPositions[2]);
    lightingShader.SetVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.SetVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.SetVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.SetFloat("pointLights[2].constant", 1.0f);
    lightingShader.SetFloat("pointLights[2].linear", 0.09f);
    lightingShader.SetFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    lightingShader.SetVector3f("pointLights[3].position", pointLightPositions[3]);
    lightingShader.SetVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.SetVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.SetVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.SetFloat("pointLights[3].constant", 1.0f);
    lightingShader.SetFloat("pointLights[3].linear", 0.09f);
    lightingShader.SetFloat("pointLights[3].quadratic", 0.032f);

    while (!app->ShouldClose())
    {
        renderer->Clear();
        renderer->ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        lightingShader.Use();
        lightingShader.SetVector3f("viewPos", camera->Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        lightingShader.SetMatrix4("projection", projection);
        lightingShader.SetMatrix4("view", view);

        // spotLight
        lightingShader.SetVector3f("spotLight.position", camera->Position);
        lightingShader.SetVector3f("spotLight.direction", camera->Front);
        lightingShader.SetVector3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.SetVector3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("spotLight.constant", 1.0f);
        lightingShader.SetFloat("spotLight.linear", 0.09f);
        lightingShader.SetFloat("spotLight.quadratic", 0.032f);
        lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        tex.Bind(0);
        tex2.Bind(1);

        cubeVAO.bind();
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);

        for(unsigned int i = 0; i < 10; i++)
        {
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.SetMatrix4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightCubeShader.Use();
        lightCubeShader.SetMatrix4("projection", projection);
        lightCubeShader.SetMatrix4("view", view);
        lightCubeVAO.bind();

        for (int i = 0; i < 4; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightCubeShader.SetMatrix4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        renderer->Present();
        app->Run();
    }
    app->Destroy();


    return 0;
}
