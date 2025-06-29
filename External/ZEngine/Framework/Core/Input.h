#pragma once

#include <unordered_map>
#include <string>
#include <array>
#include <functional>
#include <memory>
struct GLFWwindow;

namespace framework
{

    // 按键状态枚举
    enum class KeyState
    {
        None, // 未按下
        Down, // 刚按下
        Held, // 持续按下
        Up    // 刚释放
    };

    // 鼠标按键枚举
    enum class MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,
        Count
    };

    // 轴输入类型枚举 (类似Unity中的输入轴)
    enum class AxisType
    {
        KeyboardHorizontal, // A/D 或 左右箭头
        KeyboardVertical,   // W/S 或 上下箭头
        MouseX,             // 鼠标X移动
        MouseY,             // 鼠标Y移动
        MouseScrollWheel    // 鼠标滚轮
    };

    // Input类 - 单例模式
    class Input
    {
    private:
        // 私有构造函数（单例模式）
        Input();

        // Pimpl模式隐藏实现细节
        class InputImpl;
        std::unique_ptr<InputImpl> m_impl;

        // 单例实例
        static std::unique_ptr<Input> s_instance;

    public:
        ~Input();

        // 禁止拷贝和赋值
        Input(const Input &) = delete;
        Input &operator=(const Input &) = delete;

        // 获取单例实例
        static Input &GetInstance();

        // 初始化和更新
        void Initialize(GLFWwindow *window);
        void Update();

        // 键盘输入
        static bool GetKey(int keyCode);     // 是否按下（持续）
        static bool GetKeyDown(int keyCode); // 是否刚按下（当前帧）
        static bool GetKeyUp(int keyCode);   // 是否刚释放（当前帧）

        // 鼠标输入
        static bool GetMouseButton(MouseButton button);     // 是否按下（持续）
        static bool GetMouseButtonDown(MouseButton button); // 是否刚按下
        static bool GetMouseButtonUp(MouseButton button);   // 是否刚释放
        static float GetMouseX();                           // 鼠标X位置
        static float GetMouseY();                           // 鼠标Y位置
        static void GetMousePosition(float &x, float &y);   // 获取鼠标位置
        static float GetMouseScrollDelta();                 // 鼠标滚轮增量

        // 模拟Unity的输入轴
        static float GetAxis(AxisType axis);    // 获取轴的值（-1到1）
        static float GetAxisRaw(AxisType axis); // 获取轴的原始值（无平滑）

        // 自定义轴和按键映射
        void RegisterAxis(const std::string &name, AxisType axisType);
        void RegisterKeyBinding(const std::string &name, int keyCode);
        static float GetAxis(const std::string &name);
        static bool GetButton(const std::string &name);
        static bool GetButtonDown(const std::string &name);
        static bool GetButtonUp(const std::string &name);

        // 触摸输入（如果需要）
        static bool GetTouch(int touchIndex, float &x, float &y);
        static int GetTouchCount();
        
        // 内部方法 - 供回调函数使用
        void SetScrollDelta(double delta);
    };

} // namespace framework