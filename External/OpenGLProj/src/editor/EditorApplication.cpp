//
// Created by zhengshulin on 2024/6/21.
//

#include "EditorApplication.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void EditorApplication::Init() {
    Application::Init();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    frameBuffer = new FrameBuffer(context->width, context->height);
    frameBuffer->unbind();
    context->fbo = frameBuffer;

}

void EditorApplication::BeginFrame() {
    Application::BeginFrame();
    frameBuffer->bind();
}

void EditorApplication::EndFrame() {
    Application::EndFrame();

    frameBuffer->unbind();
    renderer->Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static ImGuiID id = ImGui::GetID("EditorPanelDockSpace");
    ImGui::DockSpaceOverViewport(id, ImGui::GetMainViewport());
    if (editorScene) {
        editorScene->SetGameBuffer(GetContext()->fbo);
        editorScene->RenderWidget();
    }
    ImGui::Render();


    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

EditorApplication::~EditorApplication() {
    delete editorScene;
}

EditorApplication::EditorApplication(int width, int height, const char *title) : Application(width, height, title) {
    editorScene = new EditorScene();
    editorScene->OnCreate();
}

void EditorApplication::SetScene(Scene *scene) {
    Application::SetScene(scene);
    editorScene->setRunningScene(scene);
}

EditorApplication *EditorApplication::GetInstance() {
    return Instance;
}

EditorApplication *EditorApplication::Instance = nullptr;

void EditorApplication::OnExit() {
    Application::OnExit();
    frameBuffer->destroy();
}
