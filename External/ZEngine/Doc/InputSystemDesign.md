# Input 系统设计说明

本文档描述了ZEngine的Input系统的设计和使用方法。这个系统借鉴了Unity的Input API，提供了一个易用且统一的输入处理接口。

## 设计理念

1. **统一接口**：提供处理键盘、鼠标和其他输入设备的统一接口
2. **状态管理**：跟踪输入状态，区分按下、持续按下和释放
3. **抽象轴输入**：提供类似Unity的Input.GetAxis的抽象输入轴
4. **可扩展性**：支持自定义按键映射和轴配置
5. **封装实现细节**：使用Pimpl模式隐藏底层实现细节

## 主要功能

### 键盘输入

```cpp
// 检查键是否被按下或持续按下
if (Input::GetKey(GLFW_KEY_SPACE)) {
    // 空格键被按下或持续按下
}

// 仅在键刚被按下的帧检测
if (Input::GetKeyDown(GLFW_KEY_E)) {
    // E键刚被按下
}

// 仅在键刚被释放的帧检测
if (Input::GetKeyUp(GLFW_KEY_Q)) {
    // Q键刚被释放
}
```

### 鼠标输入

```cpp
// 检查鼠标按钮
if (Input::GetMouseButton(MouseButton::Left)) {
    // 鼠标左键被按下或持续按下
}

// 获取鼠标位置
float mouseX = Input::GetMouseX();
float mouseY = Input::GetMouseY();

// 或者一次性获取鼠标位置
float x, y;
Input::GetMousePosition(x, y);

// 获取鼠标滚轮增量
float scrollDelta = Input::GetMouseScrollDelta();
```

### 轴输入

```cpp
// 获取水平轴输入（A/D或左右箭头键），范围从-1到1
float horizontal = Input::GetAxis(AxisType::KeyboardHorizontal);

// 获取垂直轴输入（W/S或上下箭头键），范围从-1到1
float vertical = Input::GetAxis(AxisType::KeyboardVertical);

// 获取鼠标X轴移动
float mouseXDelta = Input::GetAxis(AxisType::MouseX);

// 获取原始轴值（无平滑）
float rawHorizontal = Input::GetAxisRaw(AxisType::KeyboardHorizontal);
```

### 自定义输入映射

```cpp
// 注册自定义轴
Input::GetInstance().RegisterAxis("Move", AxisType::KeyboardHorizontal);
Input::GetInstance().RegisterAxis("Look", AxisType::MouseX);

// 注册自定义按键绑定
Input::GetInstance().RegisterKeyBinding("Jump", GLFW_KEY_SPACE);
Input::GetInstance().RegisterKeyBinding("Fire", GLFW_KEY_MOUSE_BUTTON_LEFT);

// 使用自定义映射
float moveAmount = Input::GetAxis("Move");
if (Input::GetButtonDown("Jump")) {
    // 执行跳跃
}
```

## 系统初始化和更新

Input系统需要在应用程序启动时初始化，并在每一帧更新：

```cpp
// 在应用程序初始化时
void BaseApplication::Initialize() {
    // ... 其他初始化代码 ...
    
    // 初始化Input系统
    Input::GetInstance().Initialize(m_window); // m_window是GLFWwindow*
    
    // ... 其他初始化代码 ...
}

// 在应用程序的主循环中
void BaseApplication::Update(float deltaTime) {
    // 更新Input系统
    Input::GetInstance().Update();
    
    // ... 其他更新代码 ...
}
```

## 内部实现

Input系统内部使用Pimpl模式，将所有实现细节隐藏在InputImpl类中。主要实现包括：

1. **状态跟踪**：维护所有键盘按键和鼠标按钮的当前和上一帧状态
2. **GLFW回调**：使用GLFW回调来捕获鼠标滚轮等事件
3. **轴值计算**：根据键盘和鼠标输入计算各种轴的值

## 扩展建议

1. **控制器支持**：添加对游戏手柄和其他控制器的支持
2. **输入动作系统**：实现一个更高级的输入动作系统，类似Unity的新Input System
3. **触摸支持**：增强对触摸屏的支持
4. **输入录制和回放**：添加记录和回放输入序列的功能，用于测试和调试
5. **热插拔设备检测**：添加对设备连接/断开的检测和处理

## 使用示例

```cpp
// 在游戏中移动角色
void PlayerController::Update(float deltaTime) {
    // 获取输入
    float horizontal = Input::GetAxis(AxisType::KeyboardHorizontal);
    float vertical = Input::GetAxis(AxisType::KeyboardVertical);
    
    // 计算移动向量
    glm::vec3 moveDirection(horizontal, 0.0f, vertical);
    
    // 归一化并应用速度
    if (glm::length(moveDirection) > 0.1f) {
        moveDirection = glm::normalize(moveDirection);
        m_position += moveDirection * m_speed * deltaTime;
    }
    
    // 处理跳跃
    if (Input::GetKeyDown(GLFW_KEY_SPACE) && m_isGrounded) {
        m_velocity.y = m_jumpForce;
    }
    
    // 处理鼠标视角旋转
    float mouseX = Input::GetAxis(AxisType::MouseX);
    float mouseY = Input::GetAxis(AxisType::MouseY);
    
    m_yaw += mouseX * m_sensitivity;
    m_pitch -= mouseY * m_sensitivity; // 注意这里是减，因为Y轴向上为正
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f); // 限制俯仰角
    
    // 更新相机旋转
    UpdateCameraRotation();
}
```
