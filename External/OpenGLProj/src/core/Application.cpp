//
// Created by zhengshulin on 2024/6/17.
//

#include "Application.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "camera.h"
#include "ui/scene.h"
#include <iostream>

 unsigned int SCR_WIDTH = 800;
 unsigned int SCR_HEIGHT = 600;
static bool s_ignoreInput= true;

// camera
Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
static Context*context;
Application *Application::GetInstance() {
    static Application app(SCR_WIDTH,SCR_HEIGHT);
    return &app;
}

bool Application::ShouldClose() {
    if (window && valid)
        return glfwWindowShouldClose(window);
    return true;
}

int Application::Run() {
    Init();

    FrameBuffer*frameBuffer=new FrameBuffer(context->width,context->height);
    frameBuffer->unbind();
    context->fbo=frameBuffer;
    while (!this->ShouldClose()){
        glfwPollEvents();

       frameBuffer->bind();
        processInput(window);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)context->width / (context->width+0.0001f), 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        context->projection=projection;
        context->view=view;

        renderer->Clear();
        if (scene){
            scene->Update();
            scene->Render(*context);
        }
     frameBuffer->unbind();
        BeginFrame();
       renderer->Clear();
        EndFrame();
        renderer->Present();

    }

    frameBuffer->destroy();
    return 0;
}

Application::~Application() {
    if (renderer)
        delete renderer;
}

void Application::Destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::Init() {
    renderer=new Renderer(context);
    context->renderer=renderer;
    renderer->SetClearMode(ClearMode::COLOR_BIT|ClearMode::DEPTH_BIT);
    renderer->Enable(FuncType::Depth_Test);
}

Application::Application(int width, int height, const char *title) {

    SCR_WIDTH=width;
    SCR_HEIGHT=height;
     lastX = SCR_WIDTH / 2.0f;
     lastY = SCR_HEIGHT / 2.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    this->window=window;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ;
    }
    this->camera=&_camera;
    valid= true;
    if (title== nullptr){
        title="ToyEngine";
    }
    context=new Context();
    context->width=width;
    context->height=height;
    context->title=title;
    context->window=window;

    glfwSetWindowTitle(window,title);

}

Context *Application::GetContext() const {
    return context;
}

int Application::GetWidth() const {
    return context->width;
}

int Application::GetHeight() const {
    return context->height;
}

void Application::SetScene(Scene *scene) {
    this->scene=scene;
    this->scene->Create();
    this->scene->SetContext(context);
}

bool Application::isIgnoreInput()  {
    return s_ignoreInput;
}

void Application::setIgnoreInput(bool ignoreInput) {
    s_ignoreInput = ignoreInput;
}


void processInput(GLFWwindow *window)
{

    //FIXME: camera move dont smooth! there has some bug! maybe glfw event loop or glfwGetKey
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (s_ignoreInput)
        return;

    static float speedUp=1.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        speedUp=4.0f;
    else
        speedUp=1.0f;

    deltaTime =deltaTime*speedUp;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        _camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        _camera.ProcessKeyboard(DOWN, deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    context->width=width;
    context->height=height;
    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (s_ignoreInput)
        return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    _camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (s_ignoreInput)
        return;
    _camera.ProcessMouseScroll(static_cast<float>(yoffset));
}