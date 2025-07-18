#include "GLFW/glfw3.h"
#include "Framework/Core/Input.h"
#include "Framework/Log/Logger.h"

namespace framework
{
    namespace
    {
        static int KeyCodeToGLFWKey(KeyCode key)
        {
            switch (key)
            {
            case KeyCode::Escape:
                return GLFW_KEY_ESCAPE;
                break;
            default:
                Logger::Error("Unsupported KeyCode: {}", static_cast<int>(key));
                return -1; // 未知按键
            }
        }
    }

    /**
     * @brief 输入系统的实现类，使用Pimpl模式隐藏实现细节
     */
    class Input::InputImpl
    {
    public:
        InputImpl() = default;
        ~InputImpl() = default;

        GLFWwindow *window = nullptr;

        std::array<KeyState, GLFW_KEY_LAST + 1> keyStates{};
        std::array<KeyState, GLFW_KEY_LAST + 1> prevKeyStates{};

        std::array<KeyState, static_cast<size_t>(MouseButton::Count)> mouseButtonStates{};
        std::array<KeyState, static_cast<size_t>(MouseButton::Count)> prevMouseButtonStates{};

        double mouseX = 0.0;
        double mouseY = 0.0;
        double prevMouseX = 0.0;
        double prevMouseY = 0.0;
        double mouseDeltaX = 0.0;
        double mouseDeltaY = 0.0;

        double scrollDelta = 0.0;

        std::unordered_map<std::string, AxisType> customAxes;
        std::unordered_map<std::string, int> customKeyBindings;

        double axisValues[5] = {0.0};

        /**
         * @brief 更新键盘状态，处理按键的按下、持续按下和释放状态
         */
        void UpdateKeyboardState()
        {
            prevKeyStates = keyStates;

            for (int key = 0; key <= GLFW_KEY_LAST; ++key)
            {
                int state = glfwGetKey(window, key);

                if (state == GLFW_PRESS)
                {
                    if (keyStates[key] == KeyState::None || keyStates[key] == KeyState::Up)
                    {
                        keyStates[key] = KeyState::Down;
                    }
                    else if (keyStates[key] == KeyState::Down)
                    {
                        keyStates[key] = KeyState::Held;
                    }
                }
                else
                {
                    if (keyStates[key] == KeyState::Down || keyStates[key] == KeyState::Held)
                    {
                        keyStates[key] = KeyState::Up;
                    }
                    else if (keyStates[key] == KeyState::Up)
                    {
                        keyStates[key] = KeyState::None;
                    }
                }
            }
        }

        /**
         * @brief 更新鼠标状态，包括位置、按钮和滚轮
         */
        void UpdateMouseState()
        {
            prevMouseButtonStates = mouseButtonStates;

            prevMouseX = mouseX;
            prevMouseY = mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            mouseDeltaX = mouseX - prevMouseX;
            mouseDeltaY = mouseY - prevMouseY;

            for (int button = 0; button < static_cast<int>(MouseButton::Count); ++button)
            {
                int state = glfwGetMouseButton(window, button);

                if (state == GLFW_PRESS)
                {
                    if (mouseButtonStates[button] == KeyState::None || mouseButtonStates[button] == KeyState::Up)
                    {
                        mouseButtonStates[button] = KeyState::Down;
                    }
                    else if (mouseButtonStates[button] == KeyState::Down)
                    {
                        mouseButtonStates[button] = KeyState::Held;
                    }
                }
                else
                {
                    if (mouseButtonStates[button] == KeyState::Down || mouseButtonStates[button] == KeyState::Held)
                    {
                        mouseButtonStates[button] = KeyState::Up;
                    }
                    else if (mouseButtonStates[button] == KeyState::Up)
                    {
                        mouseButtonStates[button] = KeyState::None;
                    }
                }
            }

            scrollDelta = 0.0;
        }

        /**
         * @brief 更新轴值，包括键盘轴（WASD/方向键）和鼠标轴
         */
        void UpdateAxisValues()
        {
            float horizontal = 0.0f;
            if (keyStates[GLFW_KEY_A] == KeyState::Down || keyStates[GLFW_KEY_A] == KeyState::Held)
                horizontal -= 1.0f;
            if (keyStates[GLFW_KEY_D] == KeyState::Down || keyStates[GLFW_KEY_D] == KeyState::Held)
                horizontal += 1.0f;
            if (keyStates[GLFW_KEY_LEFT] == KeyState::Down || keyStates[GLFW_KEY_LEFT] == KeyState::Held)
                horizontal -= 1.0f;
            if (keyStates[GLFW_KEY_RIGHT] == KeyState::Down || keyStates[GLFW_KEY_RIGHT] == KeyState::Held)
                horizontal += 1.0f;
            horizontal = horizontal > 1.0f ? 1.0f : (horizontal < -1.0f ? -1.0f : horizontal);
            axisValues[static_cast<int>(AxisType::KeyboardHorizontal)] = horizontal;

            float vertical = 0.0f;
            if (keyStates[GLFW_KEY_W] == KeyState::Down || keyStates[GLFW_KEY_W] == KeyState::Held)
                vertical += 1.0f;
            if (keyStates[GLFW_KEY_S] == KeyState::Down || keyStates[GLFW_KEY_S] == KeyState::Held)
                vertical -= 1.0f;
            if (keyStates[GLFW_KEY_UP] == KeyState::Down || keyStates[GLFW_KEY_UP] == KeyState::Held)
                vertical += 1.0f;
            if (keyStates[GLFW_KEY_DOWN] == KeyState::Down || keyStates[GLFW_KEY_DOWN] == KeyState::Held)
                vertical -= 1.0f;
            vertical = vertical > 1.0f ? 1.0f : (vertical < -1.0f ? -1.0f : vertical);
            axisValues[static_cast<int>(AxisType::KeyboardVertical)] = vertical;

            axisValues[static_cast<int>(AxisType::MouseX)] = mouseDeltaX / 10.0;
            axisValues[static_cast<int>(AxisType::MouseY)] = mouseDeltaY / 10.0;

            axisValues[static_cast<int>(AxisType::MouseScrollWheel)] = scrollDelta;
        }
    };

    /**
     * @brief 鼠标滚轮回调函数，由GLFW调用
     * @param window GLFW窗口指针
     * @param xoffset 水平滚动值
     * @param yoffset 垂直滚动值
     */
    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Input::GetInstance().SetScrollDelta(yoffset);
    }

    std::unique_ptr<Input> Input::s_instance = nullptr;

    /**
     * @brief 构造函数，初始化Pimpl对象
     */
    Input::Input() : m_impl(std::make_unique<InputImpl>())
    {
    }

    Input::~Input() = default;

    /**
     * @brief 获取Input单例实例
     * @return Input& 输入系统的单例引用
     */
    Input &Input::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<Input>(new Input());
        }
        return *s_instance;
    }

    /**
     * @brief 初始化输入系统
     * @param window GLFW窗口指针
     */
    void Input::Initialize(IWindow *window)
    {
        m_impl->window = static_cast<GLFWwindow *>(window->GetNativeWindowHandle());
        glfwSetScrollCallback(m_impl->window, ScrollCallback);
        Logger::Log("Input system initialized");
    }

    /**
     * @brief 更新输入系统状态，应每帧调用一次
     */
    void Input::Update()
    {
        if (!m_impl->window)
        {
            Logger::Warn("Input system not initialized with a window");
            return;
        }

        m_impl->UpdateKeyboardState();
        m_impl->UpdateMouseState();
        m_impl->UpdateAxisValues();
    }

    /***
     * @brief 检查按键是否被按下或持续按下
     * @param code KeyCode 枚举值
     * @return bool 如果按键处于Down或Held状态返回true
     */
    bool Input::GetKey(KeyCode code)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;
        int keyCode = KeyCodeToGLFWKey(code);
        if (keyCode < 0 || keyCode >= static_cast<int>(instance.m_impl->keyStates.size()))
        {
            Logger::Error("Invalid KeyCode: {}", static_cast<int>(code));
            return false;
        }
        return instance.m_impl->keyStates[keyCode] == KeyState::Down ||
               instance.m_impl->keyStates[keyCode] == KeyState::Held;
    }

    /**
     * @brief 检查按键是否被按下或持续按下
     * @param keyCode GLFW键码
     * @return bool 如果按键处于Down或Held状态返回true
     */
    bool Input::GetKey(int keyCode)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->keyStates[keyCode] == KeyState::Down ||
               instance.m_impl->keyStates[keyCode] == KeyState::Held;
    }

    /**
     * @brief 检查按键是否刚刚被按下
     * @param key 按键枚举
     * @return bool 如果按键处于Down状态返回true
     */
    bool Input::GetKeyDown(KeyCode key)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        int keyCode = KeyCodeToGLFWKey(key);
        if (keyCode < 0 || keyCode >= static_cast<int>(instance.m_impl->keyStates.size()))
        {
            Logger::Error("Invalid KeyCode: {}", static_cast<int>(key));
            return false;
        }
        return instance.m_impl->keyStates[keyCode] == KeyState::Down;
    }

    /**
     * @brief 检查按键是否刚刚被按下
     * @param keyCode GLFW键码
     * @return bool 如果按键处于Down状态返回true
     */
    bool Input::GetKeyDown(int keyCode)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->keyStates[keyCode] == KeyState::Down;
    }

    /**
     * @brief 检查按键是否刚刚被释放
     * @param key 按键枚举
     * @return bool 如果按键处于Up状态返回true
     */
    bool Input::GetKeyUp(KeyCode key)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        int keyCode = KeyCodeToGLFWKey(key);
        if (keyCode < 0 || keyCode >= static_cast<int>(instance.m_impl->keyStates.size()))
        {
            Logger::Error("Invalid KeyCode: {}", static_cast<int>(key));
            return false;
        }
        return instance.m_impl->keyStates[keyCode] == KeyState::Up;
    }

    /**
     * @brief 检查按键是否刚刚被释放
     * @param keyCode GLFW键码
     * @return bool 如果按键处于Up状态返回true
     */
    bool Input::GetKeyUp(int keyCode)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->keyStates[keyCode] == KeyState::Up;
    }

    /**
     * @brief 检查鼠标按钮是否被按下或持续按下
     * @param button 鼠标按钮枚举
     * @return bool 如果按钮处于Down或Held状态返回true
     */
    bool Input::GetMouseButton(MouseButton button)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Down ||
               instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Held;
    }

    /**
     * @brief 检查鼠标按钮是否刚刚被按下
     * @param button 鼠标按钮枚举
     * @return bool 如果按钮处于Down状态返回true
     */
    bool Input::GetMouseButtonDown(MouseButton button)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Down;
    }

    /**
     * @brief 检查鼠标按钮是否刚刚被释放
     * @param button 鼠标按钮枚举
     * @return bool 如果按钮处于Up状态返回true
     */
    bool Input::GetMouseButtonUp(MouseButton button)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Up;
    }

    /**
     * @brief 获取鼠标X坐标
     * @return float 鼠标当前X坐标
     */
    float Input::GetMouseX()
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        return static_cast<float>(instance.m_impl->mouseX);
    }

    /**
     * @brief 获取鼠标Y坐标
     * @return float 鼠标当前Y坐标
     */
    float Input::GetMouseY()
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        return static_cast<float>(instance.m_impl->mouseY);
    }

    /**
     * @brief 获取鼠标位置
     * @param x 用于存储X坐标的引用
     * @param y 用于存储Y坐标的引用
     */
    void Input::GetMousePosition(float &x, float &y)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
        {
            x = 0.0f;
            y = 0.0f;
            return;
        }

        x = static_cast<float>(instance.m_impl->mouseX);
        y = static_cast<float>(instance.m_impl->mouseY);
    }

    /**
     * @brief 获取鼠标滚轮增量
     * @return float 当前帧的滚轮增量
     */
    float Input::GetMouseScrollDelta()
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        return static_cast<float>(instance.m_impl->scrollDelta);
    }

    /**
     * @brief 获取输入轴的值
     * @param axis 轴类型枚举
     * @return float 轴值，范围从-1到1
     */
    float Input::GetAxis(AxisType axis)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        return static_cast<float>(instance.m_impl->axisValues[static_cast<int>(axis)]);
    }

    /**
     * @brief 获取输入轴的原始值，不使用平滑处理
     * @param axis 轴类型枚举
     * @return float 对于键盘轴返回-1、0或1，对于其他轴返回原始值
     */
    float Input::GetAxisRaw(AxisType axis)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        float value = static_cast<float>(instance.m_impl->axisValues[static_cast<int>(axis)]);
        if (axis == AxisType::KeyboardHorizontal || axis == AxisType::KeyboardVertical)
        {
            if (value > 0.1f)
                return 1.0f;
            if (value < -0.1f)
                return -1.0f;
            return 0.0f;
        }
        return value;
    }

    /**
     * @brief 注册自定义轴映射
     * @param name 轴的名称
     * @param axisType 轴类型枚举
     */
    void Input::RegisterAxis(const std::string &name, AxisType axisType)
    {
        auto &instance = GetInstance();
        instance.m_impl->customAxes[name] = axisType;
    }

    /**
     * @brief 注册自定义按键映射
     * @param name 按键的名称
     * @param keyCode GLFW键码
     */
    void Input::RegisterKeyBinding(const std::string &name, int keyCode)
    {
        auto &instance = GetInstance();
        instance.m_impl->customKeyBindings[name] = keyCode;
    }

    /**
     * @brief 根据名称获取轴的值
     * @param name 自定义轴的名称
     * @return float 轴值，范围从-1到1，若未找到则返回0
     */
    float Input::GetAxis(const std::string &name)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return 0.0f;

        auto it = instance.m_impl->customAxes.find(name);
        if (it != instance.m_impl->customAxes.end())
        {
            return GetAxis(it->second);
        }

        Logger::Warn("Axis not found: {}", name);
        return 0.0f;
    }

    /**
     * @brief 根据名称检查按键是否被按下
     * @param name 自定义按键的名称
     * @return bool 如果按键被按下返回true，若未找到则返回false
     */
    bool Input::GetButton(const std::string &name)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        auto it = instance.m_impl->customKeyBindings.find(name);
        if (it != instance.m_impl->customKeyBindings.end())
        {
            return GetKey(it->second);
        }

        Logger::Warn("Button not found: {}", name);

        return false;
    }

    /**
     * @brief 根据名称检查按键是否刚刚被按下
     * @param name 自定义按键的名称
     * @return bool 如果按键刚刚被按下返回true，若未找到则返回false
     */
    bool Input::GetButtonDown(const std::string &name)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        auto it = instance.m_impl->customKeyBindings.find(name);
        if (it != instance.m_impl->customKeyBindings.end())
        {
            return GetKeyDown(it->second);
        }

        Logger::Warn("Button not found: {}", name);
        return false;
    }

    /**
     * @brief 根据名称检查按键是否刚刚被释放
     * @param name 自定义按键的名称
     * @return bool 如果按键刚刚被释放返回true，若未找到则返回false
     */
    bool Input::GetButtonUp(const std::string &name)
    {
        auto &instance = GetInstance();
        if (!instance.m_impl->window)
            return false;

        auto it = instance.m_impl->customKeyBindings.find(name);
        if (it != instance.m_impl->customKeyBindings.end())
        {
            return GetKeyUp(it->second);
        }

        Logger::Warn("Button not found: {}", name);
        return false;
    }

    /**
     * @brief 获取触摸输入（基础实现，可根据平台扩展）
     * @param touchIndex 触摸索引
     * @param x 用于存储X坐标的引用
     * @param y 用于存储Y坐标的引用
     * @return bool 如果触摸有效返回true
     */
    bool Input::GetTouch(int touchIndex, float &x, float &y)
    {
        x = 0.0f;
        y = 0.0f;
        return false;
    }

    /**
     * @brief 获取当前触摸数量
     * @return int 当前触摸数量
     */
    int Input::GetTouchCount()
    {
        return 0;
    }

    /**
     * @brief 设置滚轮增量值（由滚轮回调函数调用）
     * @param delta 滚轮增量值
     */
    void Input::SetScrollDelta(double delta)
    {
        m_impl->scrollDelta = delta;
    }

} // namespace framework
