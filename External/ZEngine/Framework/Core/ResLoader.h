#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

namespace framework {

class ResLoader
{
private:
    // 资源根路径
    std::filesystem::path m_resRootPath;
    
    // 搜索路径列表，按优先级排序
    std::vector<std::filesystem::path> m_searchPaths;
    
    // 单例实例
    static std::unique_ptr<ResLoader> s_instance;


    // 初始化搜索路径
    void InitializeSearchPaths();

public:
    ResLoader();
    ~ResLoader() = default;
    
    // 获取单例实例
    static ResLoader& GetInstance();
    
    // 加载文本文件
    std::string LoadText(const std::string& relativePath);
    
    // 加载二进制文件
    std::vector<uint8_t> LoadBinary(const std::string& relativePath);
    
    // 检查文件是否存在
    bool FileExists(const std::string& relativePath);
    
    // 设置自定义资源根路径
    void SetResourceRootPath(const std::string& path);
    
    // 添加搜索路径
    void AddSearchPath(const std::string& path);

    // Initialize with base paths
    void Initialize(const std::string& assetsPath = "Assets/");
    
        // 查找文件的实际路径
    std::filesystem::path FindResourcePath(const std::string& relativePath);
    
    // Get full path for a resource
    std::string GetResourcePath(const std::string& relativePath) const;
    
    // Get texture path with fallback search
    std::string GetTexturePath(const std::string& filename) const;
    
    // Set resource directories
    void SetTexturesDirectory(const std::string& path);
    void SetShadersDirectory(const std::string& path);
    void SetModelsDirectory(const std::string& path);

private:
    std::string m_assetsBasePath;
    std::string m_texturesPath;
    std::string m_shadersPath;
    std::string m_modelsPath;
    
    // Helper function to find file in multiple locations
    std::string FindFile(const std::string& filename, const std::vector<std::string>& searchPaths) const;
};

} // namespace framework
