

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

#include <GLFW/glfw3.h>
#include <iostream>

// 窗口大小变化回调函数
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 更新视口大小
    glViewport(0, 0, width, height);
}

int main00() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建窗口和 OpenGL 上下文
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置窗口大小变化回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 将当前窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 初始化 GLEW 来获取 OpenGL 函数指针
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 输入事件处理
        glfwPollEvents();

        // 渲染指令
        // 清空颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制矩形
        float vertices[] = {
                // 位置
                0.5f,  0.5f, 0.0f,  // 右上角
                0.5f, -0.5f, 0.0f,  // 右下角
                -0.5f, -0.5f, 0.0f,  // 左下角
                -0.5f,  0.5f, 0.0f   // 左上角
        };
        unsigned int indices[] = {
                0, 1, 3,  // 第一个三角形
                1, 2, 3   // 第二个三角形
        };

        // 顶点数组对象
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // 顶点缓冲对象
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 索引缓冲对象
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 绘制模式
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 释放资源
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        // 交换缓冲区并检查事件
        glfwSwapBuffers(window);
    }

    // 清理资源
    glfwTerminate();
    return 0;
}
