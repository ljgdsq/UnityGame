#include "Framework/Core/ResLoader.h"
#include "Framework/Log/Logger.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

// 初始化静态成员
std::unique_ptr<ResLoader> ResLoader::s_instance = nullptr;

ResLoader::ResLoader()
{
    InitializeSearchPaths();
}

ResLoader& ResLoader::GetInstance()
{
    if (!s_instance)
    {
        s_instance = std::make_unique<ResLoader>();
    }
    return *s_instance;
}

void ResLoader::InitializeSearchPaths()
{
    // 策略1: 相对于可执行文件的路径
    // ./Res - 如果Res目录被复制到可执行文件所在目录
    m_searchPaths.push_back(std::filesystem::current_path() / "Res");
    
    // 策略2: 相对于项目根目录的路径 (针对开发环境)
    // 尝试向上查找几级目录
    auto currentPath = std::filesystem::current_path();
    for (int i = 0; i < 3; ++i) {
        auto testPath = currentPath / "Res";
        if (std::filesystem::exists(testPath)) {
            m_searchPaths.push_back(testPath);
            break;
        }
        // 向上一级目录
        if (currentPath.has_parent_path()) {
            currentPath = currentPath.parent_path();
        } else {
            break;
        }
    }
    
    // 策略3: 环境变量 (可选)
    const char* envPath = std::getenv("ZENGINE_RES_PATH");
    if (envPath) {
        m_searchPaths.push_back(envPath);
    }
    
    // 检查是否找到有效的资源路径
    for (const auto& path : m_searchPaths) {
        if (std::filesystem::exists(path)) {
            m_resRootPath = path;
            Logger::Log("Resource path found: {}", m_resRootPath.string());
            return;
        }
    }
    
    // 默认使用第一个路径
    if (!m_searchPaths.empty()) {
        m_resRootPath = m_searchPaths[0];
    }
    
    Logger::Warn("No valid resource path found!");
}

std::filesystem::path ResLoader::FindResourcePath(const std::string& relativePath)
{
    // 首先检查是否为绝对路径
    std::filesystem::path path(relativePath);
    if (path.is_absolute() && std::filesystem::exists(path)) {
        return path;
    }
    
    // 使用主资源路径
    auto fullPath = m_resRootPath / relativePath;
    if (std::filesystem::exists(fullPath)) {
        return fullPath;
    }
    
    // 按顺序检查所有搜索路径
    for (const auto& searchPath : m_searchPaths) {
        fullPath = searchPath / relativePath;
        if (std::filesystem::exists(fullPath)) {
            return fullPath;
        }
    }
    
    // 没有找到文件
    return {};
}

std::string ResLoader::LoadText(const std::string& relativePath)
{
    auto fullPath = FindResourcePath(relativePath);
    if (fullPath.empty()) {
        Logger::Error("Could not find resource file: {}", relativePath);
        return {};
    }
    
    std::ifstream file(fullPath, std::ios::in);
    if (!file.is_open()) {
        Logger::Error("Could not open file: {}", fullPath.string());
        return {};
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<uint8_t> ResLoader::LoadBinary(const std::string& relativePath)
{
    auto fullPath = FindResourcePath(relativePath);
    if (fullPath.empty()) {
        Logger::Error("Could not find resource file: {}", relativePath);
        return {};
    }
    
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Logger::Error("Could not open file: {}", fullPath.string());
        return {};
    }
    
    // 获取文件大小
    auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 分配内存并读取文件内容
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    
    return buffer;
}

bool ResLoader::FileExists(const std::string& relativePath)
{
    return !FindResourcePath(relativePath).empty();
}

void ResLoader::SetResourceRootPath(const std::string& path)
{
    auto newPath = std::filesystem::path(path);
    if (std::filesystem::exists(newPath)) {
        m_resRootPath = newPath;
    } else {
        Logger::Error("Invalid resource path: {}", path);
    }
}

void ResLoader::AddSearchPath(const std::string& path)
{
    m_searchPaths.push_back(std::filesystem::path(path));
}
