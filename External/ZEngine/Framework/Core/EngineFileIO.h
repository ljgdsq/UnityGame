#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include "Framework/Common/Define.h"
namespace framework
{

    /**
     * @brief EngineFileIO 提供静态方法用于文件输入输出操作
     * 主要用于加载文本和二进制文件，检查文件是否存在等功能。
     */
    class EngineFileIO
    {
        STATIC_CLASS(EngineFileIO);

    public:
        // 加载文本文件
        static std::string LoadText(const std::string &path);

        // 加载二进制文件
        static std::vector<uint8_t> LoadBinary(const std::string &path);

        // 检查文件是否存在
        static bool FileExists(const std::string &path);

        // 解析文件路径，返回实际的文件路径
        static std::optional<std::filesystem::path> ResolveFilePath(const std::string &path);

        // 添加查找资源路径
        static void AddSearchPath(const std::string &path);

        // 获取所有搜索路径
        static const std::vector<std::filesystem::path> &GetSearchPaths();
        // 清除所有搜索路径
        static void ClearSearchPaths();

        // 保存文本到文件
        static bool SaveText(const std::string &path, const std::string &content);

        // 列出目录下的所有文件
        static std::vector<std::filesystem::path> ListFilesInDirectory(const std::string &directory, const std::string &extension = "");

    private:
        // 查找资源文件的实际路径
        static std::optional<std::filesystem::path> FindResourcePath(const std::string &path);

        // 存储搜索路径
        static std::vector<std::filesystem::path> searchPaths;
    };
} // namespace framework
