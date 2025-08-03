#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Framework/Log/Logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Framework/Core/SceneManager.h"
#include "TestEditorApplication.h"
#include "Framework/Graphic/Buffer.h"
#include "Framework/Component/ComponentRegistration.h"
#include "TestScenes/CameraManagerExampleScene.h"
#include "TestScenes/BasicTestScene.h"
#include "Framework/Editor/Log/EditorLogSink.h"
#include "Framework/Editor/EditorContext.h"

framework::TestEditorApplication::TestEditorApplication()
{
    hierarchyWidget = new editor::HierarchyWidget();
    inspectorWidget = new editor::Inspector();
    contentBrowser = new editor::ContentBrowser();
    editorMenuBar = new editor::EditorMenuBar();
    logView = new editor::LogView();
    auto logSink = std::make_shared<editor::EditorLogSink>();
    logView->SetLogSink(logSink);
    Logger::Init({logSink});
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
    logView->Initialize();
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
    // 设置视口为 FrameBuffer 的大小
    glViewport(0, 0, gameFrameBuffer->GetWidth(), gameFrameBuffer->GetHeight());
}
#include "ImGuizmo.h"

void framework::TestEditorApplication::EndFrame()
{
    gameFrameBuffer->UnbindBuffer();
    // 恢复主窗口的视口
    glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
    m_renderer->Clear();

    // 开始ImGui渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame(); // 移到这里，每帧开始时调用
    static ImGuiID id = ImGui::GetID("EditorPanelDockSpace");
    ImGui::DockSpaceOverViewport(id, ImGui::GetMainViewport());
    ImGuiIO &io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    logView->Render();

    contentBrowser->Render();
    editorMenuBar->Render();
    gameView->Render();
    hierarchyWidget->Render();
    inspectorWidget->Render();

    ImGui::Begin("state");
    ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("FBO size: %dx%d", gameFrameBuffer->GetWidth(), gameFrameBuffer->GetHeight());
    ImGui::Text("Color texture ID: %u", gameFrameBuffer->GetColorBuffer());
    ImGui::Text("Drag : %s", editor::EditorContext::GetInstance().isDrag ? "true" : "false");
    auto selectGo = editor::EditorContext::GetInstance().GetSelectedGameObject();
    auto asset = editor::EditorContext::GetInstance().GetSelectedAsset();
    if (selectGo)
    {
        ImGui::Text("Select Object: %s", selectGo->GetName().c_str());
    }
    else if (asset)
    {
        ImGui::Text("Select Asset: %s", asset->GetName().c_str());
    }

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

    if (editorMenuBar)
    {
        editorMenuBar->Update(deltaTime);
    }

    if (contentBrowser)
    {
        contentBrowser->Update(deltaTime);
    }

    if (logView)
    {
        logView->Update(deltaTime);
    }
}
