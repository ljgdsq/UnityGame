# ZEngine UI渲染框架设计文档

## 1. 概述

本文档设计了一个完整的UI渲染框架，用于在ZEngine中实现2D UI元素的渲染。该框架将与现有的3D渲染系统协同工作，支持层级渲染、事件处理、布局管理等功能。

## 2. 设计目标

- **分离关注点**: UI渲染与3D渲染完全分离
- **性能优化**: 使用批量渲染和深度排序减少draw call
- **灵活布局**: 支持相对和绝对定位，响应式布局
- **事件处理**: 完整的UI交互系统
- **可扩展性**: 支持自定义UI组件和渲染器

## 3. 核心架构

### 3.1 渲染层次结构

```
UIManager (单例)
├── UICamera (正交相机)
├── UICanvas (画布系统)
│   ├── UILayer (渲染层)
│   │   ├── UIElement (UI元素基类)
│   │   │   ├── UIImage (图片)
│   │   │   ├── UIText (文本)
│   │   │   ├── UIButton (按钮)
│   │   │   └── UIPanel (面板)
│   │   └── UILayout (布局系统)
│   └── UIEventSystem (事件系统)
└── UIRenderer (UI专用渲染器)
```

### 3.2 渲染管线

1. **收集阶段**: 收集所有可见UI元素
2. **排序阶段**: 按层级和深度排序
3. **批量阶段**: 合并相同材质的元素
4. **渲染阶段**: 使用正交投影渲染UI

## 4. 核心组件设计

### 4.1 UICamera (UI专用相机)

```cpp
class UICamera : public Camera {
private:
    int renderOrder = 1000;        // 渲染顺序(高于3D相机)
    bool isUICamera = true;        // 标记为UI相机
    ClearFlags clearFlags = Color; // 清除标志
    
public:
    // 设置UI相机专用的正交投影
    void SetupUIProjection(float screenWidth, float screenHeight);
    
    // 屏幕坐标转换
    glm::vec2 ScreenToUIPoint(glm::vec2 screenPos);
    glm::vec2 UIPointToScreen(glm::vec2 uiPos);
};
```

**特点**:
- 使用正交投影 (0, screenWidth, 0, screenHeight)
- 高渲染优先级，在3D内容之后渲染
- 专门的坐标转换函数

### 4.2 UICanvas (画布系统)

```cpp
enum class CanvasRenderMode {
    ScreenSpaceOverlay,    // 屏幕空间覆盖
    ScreenSpaceCamera,     // 屏幕空间相机
    WorldSpace            // 世界空间
};

class UICanvas : public ComponentBase<UICanvas> {
private:
    CanvasRenderMode renderMode = ScreenSpaceOverlay;
    int sortingOrder = 0;         // 画布排序顺序
    UICamera* targetCamera = nullptr;
    glm::vec2 referenceResolution = {1920, 1080};
    float scaleFactor = 1.0f;
    
public:
    // 添加/移除UI元素
    void AddUIElement(UIElement* element);
    void RemoveUIElement(UIElement* element);
    
    // 获取所有UI元素（按渲染顺序）
    std::vector<UIElement*> GetUIElementsInRenderOrder();
    
    // 坐标转换
    glm::vec2 TransformPoint(glm::vec2 localPoint);
};
```

### 4.3 UIElement (UI元素基类)

```cpp
class UIElement : public ComponentBase<UIElement> {
protected:
    // 变换相关
    glm::vec2 anchoredPosition = {0, 0};  // 锚点位置
    glm::vec2 sizeDelta = {100, 100};     // 尺寸
    glm::vec2 anchorMin = {0.5f, 0.5f};   // 锚点最小值
    glm::vec2 anchorMax = {0.5f, 0.5f};   // 锚点最大值
    glm::vec2 pivot = {0.5f, 0.5f};       // 轴心点
    
    // 渲染相关
    int sortingOrder = 0;                 // 渲染顺序
    bool isVisible = true;                // 是否可见
    float alpha = 1.0f;                   // 透明度
    
    // 交互相关
    bool raycastTarget = true;            // 是否接收射线检测
    
public:
    // 变换操作
    virtual glm::vec2 GetWorldPosition();
    virtual glm::vec2 GetSize();
    virtual void SetAnchoredPosition(glm::vec2 pos);
    virtual void SetSize(glm::vec2 size);
    
    // 渲染接口
    virtual void OnRender(UIRenderer* renderer) = 0;
    virtual bool IsVisible() const { return isVisible && alpha > 0; }
    
    // 事件接口
    virtual bool OnPointerEnter(const UIPointerEvent& event) { return false; }
    virtual bool OnPointerExit(const UIPointerEvent& event) { return false; }
    virtual bool OnPointerDown(const UIPointerEvent& event) { return false; }
    virtual bool OnPointerUp(const UIPointerEvent& event) { return false; }
};
```

### 4.4 具体UI组件

#### UIImage (图片组件)
```cpp
class UIImage : public UIElement {
private:
    Texture* texture = nullptr;
    glm::vec4 color = {1, 1, 1, 1};      // 颜色调制
    UIImageType imageType = Simple;       // 图片类型
    bool preserveAspect = false;          // 保持宽高比
    
public:
    void SetTexture(Texture* tex);
    void SetColor(glm::vec4 newColor);
    void OnRender(UIRenderer* renderer) override;
};
```

#### UIText (文本组件)
```cpp
class UIText : public UIElement {
private:
    std::string text = "";
    Font* font = nullptr;
    float fontSize = 16.0f;
    glm::vec4 color = {1, 1, 1, 1};
    TextAlignment alignment = Center;
    bool wordWrap = false;
    
public:
    void SetText(const std::string& newText);
    void SetFont(Font* newFont);
    void OnRender(UIRenderer* renderer) override;
};
```

#### UIButton (按钮组件)
```cpp
class UIButton : public UIElement {
private:
    UIImage* backgroundImage = nullptr;
    UIText* buttonText = nullptr;
    
    // 状态样式
    ButtonState currentState = Normal;
    glm::vec4 normalColor = {1, 1, 1, 1};
    glm::vec4 hoverColor = {0.9f, 0.9f, 0.9f, 1};
    glm::vec4 pressedColor = {0.8f, 0.8f, 0.8f, 1};
    
    // 事件回调
    std::function<void()> onClickCallback = nullptr;
    
public:
    void SetBackgroundImage(Texture* texture);
    void SetText(const std::string& text);
    void SetOnClickCallback(std::function<void()> callback);
    
    bool OnPointerDown(const UIPointerEvent& event) override;
    bool OnPointerUp(const UIPointerEvent& event) override;
    void OnRender(UIRenderer* renderer) override;
};
```

### 4.5 UIRenderer (UI专用渲染器)

```cpp
class UIRenderer {
private:
    // 批量渲染数据
    struct UIBatch {
        Texture* texture = nullptr;
        std::vector<UIVertex> vertices;
        std::vector<uint32_t> indices;
        int sortingOrder = 0;
    };
    
    std::vector<UIBatch> batches;
    Shader* uiShader = nullptr;
    VertexArray* quadVAO = nullptr;
    
public:
    // 开始新的渲染帧
    void BeginFrame();
    
    // 添加UI元素到批量渲染
    void SubmitQuad(const UIRenderData& data);
    void SubmitText(const TextRenderData& data);
    
    // 执行批量渲染
    void Render(UICamera* camera);
    
    // 结束渲染帧
    void EndFrame();
    
private:
    void SortBatches();           // 按层级排序
    void MergeBatches();          // 合并相同材质
    void RenderBatch(const UIBatch& batch);
};
```

### 4.6 UIEventSystem (事件系统)

```cpp
struct UIPointerEvent {
    glm::vec2 position;           // 屏幕位置
    glm::vec2 delta;             // 移动增量
    int buttonIndex = 0;         // 按键索引
    bool isPressed = false;      // 是否按下
};

class UIEventSystem {
private:
    UIElement* currentHovered = nullptr;
    UIElement* currentPressed = nullptr;
    std::vector<UIElement*> eventTargets;
    
public:
    // 处理输入事件
    void ProcessPointerEvent(const UIPointerEvent& event);
    
    // 射线检测
    UIElement* Raycast(glm::vec2 screenPosition);
    
    // 注册/注销事件目标
    void RegisterEventTarget(UIElement* element);
    void UnregisterEventTarget(UIElement* element);
    
private:
    void UpdateHover(UIElement* newHovered, const UIPointerEvent& event);
    void ProcessClick(UIElement* target, const UIPointerEvent& event);
};
```

### 4.7 UIManager (UI管理器)

```cpp
class UIManager {
private:
    static UIManager* instance;
    
    UICamera* uiCamera = nullptr;
    UIRenderer* uiRenderer = nullptr;
    UIEventSystem* eventSystem = nullptr;
    std::vector<UICanvas*> canvases;
    
public:
    static UIManager& GetInstance();
    
    // 初始化/清理
    void Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    
    // 渲染更新
    void Update(float deltaTime);
    void Render();
    
    // Canvas管理
    void RegisterCanvas(UICanvas* canvas);
    void UnregisterCanvas(UICanvas* canvas);
    
    // 屏幕尺寸变更
    void OnScreenResize(float width, float height);
    
    // 输入处理
    void OnMouseMove(float x, float y);
    void OnMouseButton(int button, bool pressed);
};
```

## 5. 布局系统设计

### 5.1 UILayout (布局基类)
```cpp
class UILayout : public ComponentBase<UILayout> {
public:
    virtual void UpdateLayout() = 0;
    virtual void AddChild(UIElement* child) = 0;
    virtual void RemoveChild(UIElement* child) = 0;
};
```

### 5.2 具体布局类型
- **UIHorizontalLayout**: 水平布局
- **UIVerticalLayout**: 垂直布局  
- **UIGridLayout**: 网格布局
- **UIFlexLayout**: 弹性布局

## 6. 着色器设计

### 6.1 UI Vertex Shader
```glsl
#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    vTexCoord = aTexCoord;
    vColor = aColor;
    gl_Position = uProjection * uView * vec4(aPosition, 0.0, 1.0);
}
```

### 6.2 UI Fragment Shader
```glsl
#version 330 core
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;
uniform bool uUseTexture;

out vec4 FragColor;

void main() {
    if (uUseTexture) {
        FragColor = texture(uTexture, vTexCoord) * vColor;
    } else {
        FragColor = vColor;
    }
}
```

## 7. 使用示例

```cpp
// 初始化UI系统
UIManager::GetInstance().Initialize(1920, 1080);

// 创建Canvas
GameObject* canvasObj = new GameObject();
UICanvas* canvas = canvasObj->AddComponent<UICanvas>();
canvas->SetRenderMode(CanvasRenderMode::ScreenSpaceOverlay);

// 创建按钮
GameObject* buttonObj = new GameObject();
buttonObj->SetParent(canvasObj);
UIButton* button = buttonObj->AddComponent<UIButton>();
button->SetSize({200, 50});
button->SetAnchoredPosition({0, 0});
button->SetText("Click Me!");
button->SetOnClickCallback([]() {
    Logger::Log("Button clicked!");
});

// 注册到UI系统
UIManager::GetInstance().RegisterCanvas(canvas);
```

## 8. 性能优化

### 8.1 批量渲染
- 合并相同材质的UI元素
- 减少状态切换
- 使用实例化渲染

### 8.2 裁剪优化
- 视锥裁剪不可见元素
- 层级遮挡裁剪

### 8.3 内存管理
- 对象池管理UI元素
- 纹理图集减少内存占用

## 9. 文件结构

```
Framework/
├── UI/
│   ├── Core/
│   │   ├── UIManager.h/.cpp
│   │   ├── UICamera.h/.cpp
│   │   ├── UICanvas.h/.cpp
│   │   ├── UIElement.h/.cpp
│   │   └── UIRenderer.h/.cpp
│   ├── Components/
│   │   ├── UIImage.h/.cpp
│   │   ├── UIText.h/.cpp
│   │   ├── UIButton.h/.cpp
│   │   └── UIPanel.h/.cpp
│   ├── Layout/
│   │   ├── UILayout.h/.cpp
│   │   ├── UIHorizontalLayout.h/.cpp
│   │   ├── UIVerticalLayout.h/.cpp
│   │   └── UIGridLayout.h/.cpp
│   ├── Events/
│   │   ├── UIEventSystem.h/.cpp
│   │   └── UIPointerEvent.h
│   └── Rendering/
│       ├── UIShader.h/.cpp
│       └── UIBatch.h/.cpp
└── Res/
    └── Shaders/
        ├── UI.vs
        └── UI.fs
```

## 10. 实现优先级

1. **Phase 1**: 核心框架 (UIManager, UICamera, UICanvas, UIElement)
2. **Phase 2**: 基础组件 (UIImage, UIText, UIButton)  
3. **Phase 3**: 事件系统 (UIEventSystem)
4. **Phase 4**: 布局系统 (UILayout及其派生类)
5. **Phase 5**: 性能优化 (批量渲染, 裁剪)

这个设计提供了一个完整、可扩展的UI渲染框架，与现有的3D渲染系统相互独立但协同工作。

## 11. 与现有系统集成设计

### 11.1 BaseApplication集成

#### 11.1.1 初始化流程
BaseApplication需要在适当的时机初始化UI系统，确保在Renderer和Input系统之后初始化：

```cpp
void BaseApplication::Initialize()
{
    // ...existing code...
    
    // 初始化帧率管理器
    FrameRateManager::GetInstance().Initialize();
    // 默认设置为固定帧率策略
    FrameRateManager::GetInstance().SetFrameRateStrategy(FrameRateStrategy::FixedTarget);
    FrameRateManager::GetInstance().SetTargetFrameRate(60);
    
    // UI系统初始化 - 在Renderer和Input之后
    UIManager::GetInstance().Initialize(SCR_WIDTH, SCR_HEIGHT);
    UIManager::GetInstance().SetRenderer(renderer);
}
```

#### 11.1.2 渲染流程集成
在现有的渲染流程中添加UI渲染阶段：

```cpp
void BaseApplication::Run()
{
    while (!ShouldExit())
    {
        // ...existing update logic...
        
        // Render
        renderer->Clear();

        // 3D渲染阶段
        SceneManager::GetInstance().RenderActiveScene(renderer);
        
        // UI渲染阶段 - 在3D渲染之后
        UIManager::GetInstance().Render();

        renderer->SwapBuffers();
        
        // ...existing frame rate control...
    }
}
```

#### 11.1.3 更新流程集成
在主循环中添加UI系统的更新：

```cpp
void BaseApplication::Update(float deltaTime)
{
    // 更新当前场景
    SceneManager::GetInstance().UpdateActiveScene(deltaTime);
    
    // 更新UI系统
    UIManager::GetInstance().Update(deltaTime);
}
```

#### 11.1.4 输入处理集成
将鼠标和键盘事件传递给UI系统：

```cpp
void BaseApplication::HandleInput()
{
    static GLFWwindow *window = glfwGetCurrentContext();
    
    // 处理UI输入事件
    UIManager::GetInstance().ProcessInput();
    
    // 现有的应用程序输入处理
    if (Input::GetInstance().GetKeyDown(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}
```

#### 11.1.5 窗口大小变更处理
添加窗口大小变更的回调处理：

```cpp
void BaseApplication::Initialize()
{
    // ...existing code...
    
    // 设置窗口大小变更回调
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        // 更新渲染器视口
        renderer->SetViewport(0, 0, width, height);
        
        // 更新UI系统
        UIManager::GetInstance().OnScreenResize(width, height);
    });
}
```

#### 11.1.6 清理流程
在应用程序关闭时清理UI资源：

```cpp
void BaseApplication::Shutdown()
{
    SceneManager::GetInstance().ShutdownActiveScene();
    
    // 清理UI系统
    UIManager::GetInstance().Shutdown();
}
```

### 11.2 Scene集成

#### 11.2.1 Scene基类扩展
为Scene基类添加UI支持：

```cpp
class Scene {
protected:
    std::vector<UICanvas*> uiCanvases;        // 场景中的UI画布
    bool hasUI = false;                       // 标记场景是否包含UI
    
public:
    // UI相关方法
    virtual void InitializeUI() {}            // 初始化场景UI
    virtual void UpdateUI(float deltaTime) {} // 更新场景UI
    virtual void ShutdownUI() {}              // 清理场景UI
    
    // Canvas管理
    void AddUICanvas(UICanvas* canvas);
    void RemoveUICanvas(UICanvas* canvas);
    std::vector<UICanvas*> GetUICanvases() const { return uiCanvases; }
    
    // 现有方法的扩展
    virtual void Initialize() override {
        // 现有的3D场景初始化
        // ...
        
        // 初始化UI
        InitializeUI();
        
        // 注册UI Canvas到UIManager
        for (auto* canvas : uiCanvases) {
            UIManager::GetInstance().RegisterCanvas(canvas);
        }
    }
    
    virtual void Update(float deltaTime) override {
        // 现有的3D场景更新
        // ...
        
        // 更新UI
        UpdateUI(deltaTime);
    }
    
    virtual void Shutdown() override {
        // 清理UI
        ShutdownUI();
        
        // 从UIManager注销Canvas
        for (auto* canvas : uiCanvases) {
            UIManager::GetInstance().UnregisterCanvas(canvas);
        }
        
        // 现有的3D场景清理
        // ...
    }
};
```

#### 11.2.2 场景UI生命周期管理
定义清晰的UI生命周期：

```cpp
// 场景切换时的UI处理
class SceneManager {
public:
    bool SetActiveScene(const std::string& sceneName) {
        // 清理当前场景的UI
        if (currentScene) {
            currentScene->ShutdownUI();
            // 从UIManager注销所有Canvas
            for (auto* canvas : currentScene->GetUICanvases()) {
                UIManager::GetInstance().UnregisterCanvas(canvas);
            }
        }
        
        // 切换场景
        currentScene = scenes[sceneName];
        
        // 初始化新场景的UI
        if (currentScene) {
            currentScene->InitializeUI();
            // 注册新场景的Canvas
            for (auto* canvas : currentScene->GetUICanvases()) {
                UIManager::GetInstance().RegisterCanvas(canvas);
            }
        }
        
        return currentScene != nullptr;
    }
};
```

#### 11.2.3 具体场景实现示例
以AssetManagerTestScene为例展示UI集成：

```cpp
class AssetManagerTestScene : public Scene {
private:
    // UI元素
    UICanvas* mainCanvas = nullptr;
    UIPanel* infoPanel = nullptr;
    UIText* statusText = nullptr;
    UIButton* testButton1 = nullptr;
    UIButton* testButton2 = nullptr;
    
public:
    void InitializeUI() override {
        // 创建主画布
        GameObject* canvasObj = new GameObject();
        canvasObj->SetName("MainCanvas");
        mainCanvas = canvasObj->AddComponent<UICanvas>();
        mainCanvas->SetRenderMode(CanvasRenderMode::ScreenSpaceOverlay);
        mainCanvas->SetSortingOrder(0);
        AddGameObject(canvasObj);
        uiCanvases.push_back(mainCanvas);
        
        // 创建信息面板
        CreateInfoPanel();
        
        // 创建测试按钮
        CreateTestButtons();
        
        // 创建状态文本
        CreateStatusText();
    }
    
    void UpdateUI(float deltaTime) override {
        // 更新状态文本
        if (statusText) {
            std::string status = "FPS: " + std::to_string(GetCurrentFPS()) + 
                               "\nMesh Objects: " + std::to_string(m_testObjects.size());
            statusText->SetText(status);
        }
    }
    
    void ShutdownUI() override {
        // UI组件会随GameObject自动清理
        mainCanvas = nullptr;
        infoPanel = nullptr;
        statusText = nullptr;
        testButton1 = nullptr;
        testButton2 = nullptr;
    }
    
private:
    void CreateInfoPanel() {
        GameObject* panelObj = new GameObject();
        panelObj->SetName("InfoPanel");
        panelObj->SetParent(mainCanvas->GetGameObject());
        
        infoPanel = panelObj->AddComponent<UIPanel>();
        infoPanel->SetAnchoredPosition({-400, 300});  // 左上角
        infoPanel->SetSize({350, 200});
        infoPanel->SetBackgroundColor({0, 0, 0, 0.7f}); // 半透明黑色
        
        AddGameObject(panelObj);
    }
    
    void CreateTestButtons() {
        // 测试按钮1
        GameObject* button1Obj = new GameObject();
        button1Obj->SetName("TestButton1");
        button1Obj->SetParent(infoPanel->GetGameObject());
        
        testButton1 = button1Obj->AddComponent<UIButton>();
        testButton1->SetAnchoredPosition({0, 50});
        testButton1->SetSize({200, 30});
        testButton1->SetText("Test Mesh Loading");
        testButton1->SetOnClickCallback([this]() {
            TestObjMeshLoading();
        });
        
        AddGameObject(button1Obj);
        
        // 测试按钮2
        GameObject* button2Obj = new GameObject();
        button2Obj->SetName("TestButton2");
        button2Obj->SetParent(infoPanel->GetGameObject());
        
        testButton2 = button2Obj->AddComponent<UIButton>();
        testButton2->SetAnchoredPosition({0, 10});
        testButton2->SetSize({200, 30});
        testButton2->SetText("Test Asset Management");
        testButton2->SetOnClickCallback([this]() {
            TestMeshAssetCreationAndManagement();
        });
        
        AddGameObject(button2Obj);
    }
    
    void CreateStatusText() {
        GameObject* textObj = new GameObject();
        textObj->SetName("StatusText");
        textObj->SetParent(infoPanel->GetGameObject());
        
        statusText = textObj->AddComponent<UIText>();
        statusText->SetAnchoredPosition({0, -50});
        statusText->SetSize({300, 100});
        statusText->SetText("Status: Ready");
        statusText->SetFontSize(14);
        statusText->SetColor({1, 1, 1, 1});
        statusText->SetAlignment(TextAlignment::Left);
        
        AddGameObject(textObj);
    }
};
```

### 11.3 渲染管线集成

#### 11.3.1 多相机渲染支持
修改现有的渲染流程以支持多相机和UI相机：

```cpp
class Renderer {
public:
    // 渲染指定相机的内容
    void RenderWithCamera(Camera* camera) {
        // 设置相机参数
        SetViewMatrix(camera->GetViewMatrix());
        SetProjectionMatrix(camera->GetProjectionMatrix());
        
        // 设置视口
        if (camera->IsUICamera()) {
            // UI相机使用全屏视口
            SetViewport(0, 0, screenWidth, screenHeight);
        } else {
            // 3D相机可能有自定义视口
            auto viewport = camera->GetViewport();
            SetViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        }
        
        // 清除缓冲区
        if (camera->ShouldClearColor()) {
            Clear(camera->GetClearColor());
        }
        if (camera->ShouldClearDepth()) {
            ClearDepth();
        }
    }
};
```

#### 11.3.2 渲染顺序管理
确保正确的渲染顺序：

```cpp
void BaseApplication::RenderFrame() {
    // 1. 获取所有需要渲染的相机
    auto cameras = CameraManager::GetInstance().GetAllCameras();
    
    // 2. 按渲染顺序排序
    std::sort(cameras.begin(), cameras.end(), [](Camera* a, Camera* b) {
        return a->GetRenderOrder() < b->GetRenderOrder();
    });
    
    // 3. 逐个渲染相机内容
    for (auto* camera : cameras) {
        renderer->RenderWithCamera(camera);
        
        if (camera->IsUICamera()) {
            // UI相机渲染UI内容
            UIManager::GetInstance().RenderWithCamera(camera);
        } else {
            // 3D相机渲染场景内容
            SceneManager::GetInstance().RenderActiveScene(renderer);
        }
    }
}
```

### 11.4 事件系统集成

#### 11.4.1 输入事件分发
建立输入事件的优先级分发机制：

```cpp
void BaseApplication::ProcessInputEvents() {
    // 1. 首先让UI系统处理输入
    bool uiHandled = UIManager::GetInstance().ProcessInput();
    
    // 2. 如果UI没有处理，传递给场景
    if (!uiHandled) {
        SceneManager::GetInstance().ProcessInput();
    }
    
    // 3. 最后处理应用程序级别的输入
    HandleApplicationInput();
}
```

#### 11.4.2 事件回调注册
在BaseApplication中注册必要的事件回调：

```cpp
void BaseApplication::SetupEventCallbacks(GLFWwindow* window) {
    // 鼠标位置回调
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        UIManager::GetInstance().OnMouseMove(x, y);
    });
    
    // 鼠标按键回调
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        bool pressed = (action == GLFW_PRESS);
        UIManager::GetInstance().OnMouseButton(button, pressed);
    });
    
    // 键盘回调
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        UIManager::GetInstance().OnKeyEvent(key, action, mods);
    });
    
    // 窗口大小回调
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        renderer->SetViewport(0, 0, width, height);
        UIManager::GetInstance().OnScreenResize(width, height);
    });
}
```

### 11.5 资源管理集成

#### 11.5.1 UI资源加载
将UI资源集成到现有的资源管理系统：

```cpp
void ResLoader::LoadUIResources() {
    // 加载UI纹理
    LoadTexture("UI/button_normal.png");
    LoadTexture("UI/button_hover.png");
    LoadTexture("UI/button_pressed.png");
    LoadTexture("UI/panel_background.png");
    
    // 加载UI字体
    LoadFont("UI/arial.ttf");
    LoadFont("UI/consolas.ttf");
    
    // 加载UI着色器
    LoadShader("UI/ui_default.vs", "UI/ui_default.fs");
    LoadShader("UI/ui_text.vs", "UI/ui_text.fs");
}
```

#### 11.5.2 UI配置文件
支持从配置文件加载UI布局：

```cpp
// UIConfig.json
{
    "canvases": [
        {
            "name": "MainUI",
            "renderMode": "ScreenSpaceOverlay",
            "sortingOrder": 0,
            "elements": [
                {
                    "type": "UIPanel",
                    "name": "InfoPanel",
                    "position": [-400, 300],
                    "size": [350, 200],
                    "backgroundColor": [0, 0, 0, 0.7]
                }
            ]
        }
    ]
}
```

这个集成设计确保了UI系统与现有的BaseApplication和Scene系统无缝集成，同时保持了系统的模块化和可扩展性。
