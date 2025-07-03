#pragma once
#include "Framework/Component/Camera.h"
#include <vector>
#include <memory>

namespace framework {

/**
 * @brief 相机管理器类
 * 用于管理场景中的所有相机，特别是主相机
 */
class CameraManager {
public:

    ~CameraManager() = default;
    CameraManager() = default;

    /**
     * @brief 获取相机管理器单例实例
     * @return 相机管理器的引用
     */
    static CameraManager& GetInstance();
    
    /**
     * @brief 注册相机到管理器
     * @param camera 要注册的相机组件指针
     */
    void RegisterCamera(Camera* camera);
    
    /**
     * @brief 从管理器中注销相机
     * @param camera 要注销的相机组件指针
     */
    void UnregisterCamera(Camera* camera);
    
    /**
     * @brief 设置主相机
     * @param camera 要设为主相机的相机组件指针
     */
    void SetMainCamera(Camera* camera);
    
    /**
     * @brief 获取主相机
     * @return 主相机的指针，如果没有主相机则返回nullptr
     */
    Camera* GetMainCamera() const;
    
    /**
     * @brief 获取所有注册的相机
     * @return 相机组件指针的向量
     */
    const std::vector<Camera*>& GetAllCameras() const;
    
    /**
     * @brief 更新所有相机的状态
     * 在每一帧调用，处理相机的优先级变化等
     */
    void Update();
    
    /**
     * @brief 清除所有注册的相机
     * 通常在场景切换时调用
     */
    void Clear();

private:
    // 单例实现
    
    // 禁止拷贝和赋值
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;
    
    // 相机列表
    std::vector<Camera*> m_cameras;
    
    // 主相机引用
    Camera* m_mainCamera = nullptr;
    
    // 单例实例
    static std::unique_ptr<CameraManager> s_instance;
};

} // namespace framework
