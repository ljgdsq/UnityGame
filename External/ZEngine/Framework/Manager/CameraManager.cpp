// CameraManager.cpp
#include "Framework/Manager/CameraManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/GameObject.h"
namespace framework {

// 初始化静态成员
std::unique_ptr<CameraManager> CameraManager::s_instance = nullptr;

CameraManager& CameraManager::GetInstance() {
    if (!s_instance) {
        s_instance = std::make_unique<CameraManager>();
    }
    return *s_instance;
}

void CameraManager::RegisterCamera(Camera* camera) {
    if (!camera) {
        Logger::Error("CameraManager: Attempted to register a null camera");
        return;
    }
    
    // 检查是否已经注册
    for (auto* registeredCamera : m_cameras) {
        if (registeredCamera == camera) {
            // 已经注册，不需要重复添加
            return;
        }
    }
    
    // 添加到相机列表
    m_cameras.push_back(camera);
    
    // 如果这是第一个相机，或者相机被设置为主相机，则更新主相机
    if (m_cameras.size() == 1 || camera->IsMainCamera()) {
        SetMainCamera(camera);
    }
    
    Logger::Debug("CameraManager: Camera registered, total cameras: {}", m_cameras.size());
}

void CameraManager::UnregisterCamera(Camera* camera) {
    if (!camera) {
        return;
    }
    
    // 查找并移除相机
    auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
    if (it != m_cameras.end()) {
        m_cameras.erase(it);
        
        // 如果移除的是主相机，需要重新设置主相机
        if (m_mainCamera == camera) {
            m_mainCamera = nullptr;
            
            // 如果还有其他相机，选择第一个作为主相机
            if (!m_cameras.empty()) {
                SetMainCamera(m_cameras[0]);
            }
        }
        
        Logger::Debug("CameraManager: Camera unregistered, total cameras: {}", m_cameras.size());
    }
}

void CameraManager::SetMainCamera(Camera* camera) {
    if (!camera) {
        Logger::Error("CameraManager: Attempted to set a null camera as main camera");
        return;
    }
    
    // 确保相机已注册
    bool isRegistered = false;
    for (auto* registeredCamera : m_cameras) {
        if (registeredCamera == camera) {
            isRegistered = true;
            break;
        }
    }
    
    if (!isRegistered) {
        // 自动注册未注册的相机
        RegisterCamera(camera);
    }
    
    // 更新所有相机的主相机状态
    for (auto* registeredCamera : m_cameras) {
        if (registeredCamera == camera) {
            registeredCamera->SetMainCamera(true);
        } else if (registeredCamera->IsMainCamera()) {
            registeredCamera->SetMainCamera(false);
        }
    }
    
    // 更新主相机引用
    m_mainCamera = camera;
    Logger::Debug("CameraManager: Main camera set to {}", camera->GetGameObject()->GetName());
}

Camera* CameraManager::GetMainCamera() const {
    return m_mainCamera;
}

const std::vector<Camera*>& CameraManager::GetAllCameras() const {
    return m_cameras;
}

void CameraManager::Update() {
    // 在这里可以添加相机优先级处理、自动切换等逻辑
    
    // 如果没有主相机，但有注册的相机，则设置第一个为主相机
    if (!m_mainCamera && !m_cameras.empty()) {
        SetMainCamera(m_cameras[0]);
    }
}

void CameraManager::Clear() {
    m_cameras.clear();
    m_mainCamera = nullptr;
    Logger::Debug("CameraManager: All cameras cleared");
}

} // namespace framework
