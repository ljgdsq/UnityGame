#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Framework/Log/Logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Framework/Core/SceneManager.h"
#include "TestEditorApplication.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Editor/ComponentRegistration.h"
#include "TestScenes/CameraManagerExampleScene.h"
#include "TestScenes/BasicTestScene.h"

framework::TestEditorApplication::TestEditorApplication()
{
    hierarchyWidget = new editor::HierarchyWidget();
    inspectorWidget = new editor::Inspector();
    contentBrowser = new editor::ContentBrowser();
    editorMenuBar = new editor::EditorMenuBar();
}

void framework::TestEditorApplication::InitScenes()
{
    // 注册所有组件
    RegisterAllComponents();

    gameFrameBuffer = new FrameBuffer(1280, 720);

    hierarchyWidget->Initialize();
    gameView = new editor::GameView();
    gameView->SetGameFrameBuffer(gameFrameBuffer);
    gameView->Initialize();
    inspectorWidget->Initialize();
    contentBrowser->Initialize();
    editorMenuBar->Initialize();
    auto testScene = new BasicTestScene();
    testScene->SetName("BasicTestScene");
    SceneManager::GetInstance().AddScene("BasicTestScene", testScene);

    SetInitialScene("BasicTestScene");

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);

    // Use appropriate GLSL version based on OpenGL version
#ifdef __APPLE__
    // macOS with OpenGL 3.3 Core Profile should use GLSL 330
    const char *glsl_version = "#version 330 core";
#else
    // Other platforms can use GLSL 130 or 330
    const char *glsl_version = "#version 130";
#endif

    ImGui_ImplOpenGL3_Init(glsl_version);
}

void framework::TestEditorApplication::HandleInput()
{
    if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
    {
        Logger::Log("Escape key pressed, exiting application.");
        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
    }
}

void framework::TestEditorApplication::BegineFrame()
{
    gameFrameBuffer->BindBuffer();
}

void framework::TestEditorApplication::EndFrame()
{
    gameFrameBuffer->UnbindBuffer();
    m_renderer->Clear();

    // 开始ImGui渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static ImGuiID id = ImGui::GetID("EditorPanelDockSpace");
    ImGui::DockSpaceOverViewport(id, ImGui::GetMainViewport());

    hierarchyWidget->Render();
    gameView->Render();
    inspectorWidget->Render();
    contentBrowser->Render();
    editorMenuBar->Render();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Begin("state");
    ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("FBO size: %dx%d", gameFrameBuffer->GetWidth(), gameFrameBuffer->GetHeight());
    ImGui::Text("Color texture ID: %u", gameFrameBuffer->GetColorBuffer());
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void framework::TestEditorApplication::OnUpdate(float deltaTime)
{
    if (hierarchyWidget)
    {
        hierarchyWidget->Update(deltaTime);
    }

    if (gameView)
    {
        gameView->Update(deltaTime);
    }

    if (inspectorWidget)
    {
        inspectorWidget->Update(deltaTime);
    }

    if(editorMenuBar){
        editorMenuBar->Update(deltaTime);
    }
}
