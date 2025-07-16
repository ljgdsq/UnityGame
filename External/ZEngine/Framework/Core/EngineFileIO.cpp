#include <fstream>
#include <sstream>
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Log/Logger.h"
#include "Framework/Util/FileUtil.hpp"

namespace framework
{
    std::string EngineFileIO::LoadText(const std::string &path)
    {
        auto fullPath = FindResourcePath(path);
        if (!fullPath.has_value() || !std::filesystem::exists(fullPath.value()))
        {
            Logger::Error("Could not find resource file: {}", path);
            return "";
        }

        std::ifstream file(fullPath.value(), std::ios::in);
        if (!file.is_open())
        {
            Logger::Error("Failed to open file: {}", fullPath.value().string());
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<uint8_t> EngineFileIO::LoadBinary(const std::string &path)
    {
        auto fullPath = FindResourcePath(path);
        if (!fullPath.has_value() || !std::filesystem::exists(fullPath.value()))
        {
            Logger::Error("File does not exist: {}", fullPath.value().string());
            return {};
        }

        std::ifstream file(fullPath.value(), std::ios::binary);
        if (!file.is_open())
        {
            Logger::Error("Failed to open file: {}", fullPath.value().string());
            return {};
        }

        return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    bool EngineFileIO::FileExists(const std::string &path)
    {
        auto fullPath = FindResourcePath(path);
        return fullPath.has_value() && std::filesystem::exists(fullPath.value());
    }

    void EngineFileIO::AddSearchPath(const std::string &path)
    {
        searchPaths.emplace_back(path);
    }

    const std::vector<std::filesystem::path> &EngineFileIO::GetSearchPaths()
    {
        return searchPaths;
    }

    void EngineFileIO::ClearSearchPaths()
    {
        searchPaths.clear();
    }

    static std::optional<std::filesystem::path> FindResourcePath(const std::string &path)
    {
        for (const auto &searchPath : searchPaths)
        {
            auto fullPath = searchPath / path;
            if (std::filesystem::exists(fullPath))
            {
                return fullPath;
            }
        }
        return std::nullopt;
    }

} // namespace framework
