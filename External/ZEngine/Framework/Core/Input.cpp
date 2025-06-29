#include "glfw/glfw3.h"
#include "Framework/Core/Input.h"
#include "Framework/Log/Logger.h"


namespace framework {

class Input::InputImpl {
public:
    InputImpl() = default;
    ~InputImpl() = default;
    
    GLFWwindow* window = nullptr;
    
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
    
    void UpdateKeyboardState() {
        prevKeyStates = keyStates;
        
        for (int key = 0; key <= GLFW_KEY_LAST; ++key) {
            int state = glfwGetKey(window, key);
            
            if (state == GLFW_PRESS) {
                if (keyStates[key] == KeyState::None || keyStates[key] == KeyState::Up) {
                    keyStates[key] = KeyState::Down;
                } else if (keyStates[key] == KeyState::Down) {
                    keyStates[key] = KeyState::Held;
                }
            } else {
                if (keyStates[key] == KeyState::Down || keyStates[key] == KeyState::Held) {
                    keyStates[key] = KeyState::Up;
                } else if (keyStates[key] == KeyState::Up) {
                    keyStates[key] = KeyState::None;
                }
            }
        }
    }
    
    void UpdateMouseState() {
        prevMouseButtonStates = mouseButtonStates;
        
        prevMouseX = mouseX;
        prevMouseY = mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        mouseDeltaX = mouseX - prevMouseX;
        mouseDeltaY = mouseY - prevMouseY;
        
        for (int button = 0; button < static_cast<int>(MouseButton::Count); ++button) {
            int state = glfwGetMouseButton(window, button);
            
            if (state == GLFW_PRESS) {
                if (mouseButtonStates[button] == KeyState::None || mouseButtonStates[button] == KeyState::Up) {
                    mouseButtonStates[button] = KeyState::Down;
                } else if (mouseButtonStates[button] == KeyState::Down) {
                    mouseButtonStates[button] = KeyState::Held;
                }
            } else {
                if (mouseButtonStates[button] == KeyState::Down || mouseButtonStates[button] == KeyState::Held) {
                    mouseButtonStates[button] = KeyState::Up;
                } else if (mouseButtonStates[button] == KeyState::Up) {
                    mouseButtonStates[button] = KeyState::None;
                }
            }
        }
        

        scrollDelta = 0.0;
    }
    
    void UpdateAxisValues() {
        float horizontal = 0.0f;
        if (keyStates[GLFW_KEY_A] == KeyState::Down || keyStates[GLFW_KEY_A] == KeyState::Held) horizontal -= 1.0f;
        if (keyStates[GLFW_KEY_D] == KeyState::Down || keyStates[GLFW_KEY_D] == KeyState::Held) horizontal += 1.0f;
        if (keyStates[GLFW_KEY_LEFT] == KeyState::Down || keyStates[GLFW_KEY_LEFT] == KeyState::Held) horizontal -= 1.0f;
        if (keyStates[GLFW_KEY_RIGHT] == KeyState::Down || keyStates[GLFW_KEY_RIGHT] == KeyState::Held) horizontal += 1.0f;
        horizontal = horizontal > 1.0f ? 1.0f : (horizontal < -1.0f ? -1.0f : horizontal);
        axisValues[static_cast<int>(AxisType::KeyboardHorizontal)] = horizontal;
        
        float vertical = 0.0f;
        if (keyStates[GLFW_KEY_W] == KeyState::Down || keyStates[GLFW_KEY_W] == KeyState::Held) vertical += 1.0f;
        if (keyStates[GLFW_KEY_S] == KeyState::Down || keyStates[GLFW_KEY_S] == KeyState::Held) vertical -= 1.0f;
        if (keyStates[GLFW_KEY_UP] == KeyState::Down || keyStates[GLFW_KEY_UP] == KeyState::Held) vertical += 1.0f;
        if (keyStates[GLFW_KEY_DOWN] == KeyState::Down || keyStates[GLFW_KEY_DOWN] == KeyState::Held) vertical -= 1.0f;
        vertical = vertical > 1.0f ? 1.0f : (vertical < -1.0f ? -1.0f : vertical);
        axisValues[static_cast<int>(AxisType::KeyboardVertical)] = vertical;
        
        axisValues[static_cast<int>(AxisType::MouseX)] = mouseDeltaX / 10.0; 
        axisValues[static_cast<int>(AxisType::MouseY)] = mouseDeltaY / 10.0; 
        
        axisValues[static_cast<int>(AxisType::MouseScrollWheel)] = scrollDelta;
    }
};

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input::GetInstance().SetScrollDelta(yoffset);
}

std::unique_ptr<Input> Input::s_instance = nullptr;

Input::Input() : m_impl(std::make_unique<InputImpl>()) {
}

Input::~Input() = default;

Input& Input::GetInstance() {
    if (!s_instance) {
        s_instance = std::unique_ptr<Input>(new Input());
    }
    return *s_instance;
}

void Input::Initialize(GLFWwindow* window) {
    m_impl->window = window;
    
    glfwSetScrollCallback(window, ScrollCallback);
    Logger::Log("Input system initialized");
}

void Input::Update() {
    if (!m_impl->window) {
        Logger::Warn("Input system not initialized with a window");
        return;
    }
    
    m_impl->UpdateKeyboardState();
    m_impl->UpdateMouseState();
    m_impl->UpdateAxisValues();
}

bool Input::GetKey(int keyCode) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->keyStates[keyCode] == KeyState::Down || 
           instance.m_impl->keyStates[keyCode] == KeyState::Held;
}

bool Input::GetKeyDown(int keyCode) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->keyStates[keyCode] == KeyState::Down;
}

bool Input::GetKeyUp(int keyCode) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->keyStates[keyCode] == KeyState::Up;
}

bool Input::GetMouseButton(MouseButton button) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Down || 
           instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Held;
}

bool Input::GetMouseButtonDown(MouseButton button) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Down;
}

bool Input::GetMouseButtonUp(MouseButton button) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    return instance.m_impl->mouseButtonStates[static_cast<size_t>(button)] == KeyState::Up;
}

float Input::GetMouseX() {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    return static_cast<float>(instance.m_impl->mouseX);
}

float Input::GetMouseY() {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    return static_cast<float>(instance.m_impl->mouseY);
}

void Input::GetMousePosition(float& x, float& y) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) {
        x = 0.0f;
        y = 0.0f;
        return;
    }
    
    x = static_cast<float>(instance.m_impl->mouseX);
    y = static_cast<float>(instance.m_impl->mouseY);
}

float Input::GetMouseScrollDelta() {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    return static_cast<float>(instance.m_impl->scrollDelta);
}

float Input::GetAxis(AxisType axis) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    return static_cast<float>(instance.m_impl->axisValues[static_cast<int>(axis)]);
}

float Input::GetAxisRaw(AxisType axis) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    float value = static_cast<float>(instance.m_impl->axisValues[static_cast<int>(axis)]);
    if (axis == AxisType::KeyboardHorizontal || axis == AxisType::KeyboardVertical) {
        if (value > 0.1f) return 1.0f;
        if (value < -0.1f) return -1.0f;
        return 0.0f;
    }
    return value;
}

void Input::RegisterAxis(const std::string& name, AxisType axisType) {
    auto& instance = GetInstance();
    instance.m_impl->customAxes[name] = axisType;
}

void Input::RegisterKeyBinding(const std::string& name, int keyCode) {
    auto& instance = GetInstance();
    instance.m_impl->customKeyBindings[name] = keyCode;
}

float Input::GetAxis(const std::string& name) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return 0.0f;
    
    auto it = instance.m_impl->customAxes.find(name);
    if (it != instance.m_impl->customAxes.end()) {
        return GetAxis(it->second);
    }
    
    Logger::Warn("Axis not found: {}" ,name);
    return 0.0f;
}

bool Input::GetButton(const std::string& name) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    auto it = instance.m_impl->customKeyBindings.find(name);
    if (it != instance.m_impl->customKeyBindings.end()) {
        return GetKey(it->second);
    }
    
        Logger::Warn("Button not found: {}", name);

    return false;
}

bool Input::GetButtonDown(const std::string& name) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    auto it = instance.m_impl->customKeyBindings.find(name);
    if (it != instance.m_impl->customKeyBindings.end()) {
        return GetKeyDown(it->second);
    }
    
    Logger::Warn("Button not found: {}", name);
    return false;
}

bool Input::GetButtonUp(const std::string& name) {
    auto& instance = GetInstance();
    if (!instance.m_impl->window) return false;
    
    auto it = instance.m_impl->customKeyBindings.find(name);
    if (it != instance.m_impl->customKeyBindings.end()) {
        return GetKeyUp(it->second);
    }
    
    Logger::Warn("Button not found: {}", name);
    return false;
}

bool Input::GetTouch(int touchIndex, float& x, float& y) {
    x = 0.0f;
    y = 0.0f;
    return false;
}

int Input::GetTouchCount() {
    return 0;
}

void Input::SetScrollDelta(double delta)
{
    m_impl->scrollDelta = delta;
}

} // namespace framework
