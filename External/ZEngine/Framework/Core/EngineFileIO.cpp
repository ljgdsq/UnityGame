#include <fstream>
#include <sstream>
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Log/Logger.h"
#include "Framework/Util/FileUtil.hpp"
#include "EngineFileIO.h"

namespace framework
{
    std::vector<std::filesystem::path> EngineFileIO::searchPaths{std::filesystem::current_path() / "Res", std::filesystem::current_path()};

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

    std::optional<std::filesystem::path> EngineFileIO::ResolveFilePath(const std::string &path)
    {
        return FindResourcePath(path);
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

     std::optional<std::filesystem::path> EngineFileIO::FindResourcePath(const std::string &path)
    {
        auto fullPath = std::filesystem::path(path);
        if (fullPath.is_absolute() && std::filesystem::exists(fullPath))
        {
            return fullPath;
        }

        for (const auto& searchPath : EngineFileIO::searchPaths)
        {
            auto fullPath = std::filesystem::path(searchPath) / path;
            if (std::filesystem::exists(fullPath))
            {
                return fullPath;
            }
        }
        return std::nullopt;
    }

    bool EngineFileIO::SaveText(const std::string &path, const std::string &content)
    {
        std::filesystem::path fullPath = path;
        if (!fullPath.is_absolute())
        {
            fullPath = std::filesystem::current_path() / path;
        }

        std::ofstream file(fullPath, std::ios::out);
        if (!file.is_open())
        {
            Logger::Error("Failed to open file for writing: {}", fullPath.string());
            return false;
        }

        file << content;
        return true;
    }

    std::vector<std::filesystem::path> EngineFileIO::ListFilesInDirectory(const std::string &directory, const std::string &extension)
    {
        std::vector<std::filesystem::path> files;
        std::filesystem::path dirPath(directory);
        if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
        {
            Logger::Error("Invalid directory: {}", directory);
            return files;
        }

        for (const auto &entry : std::filesystem::directory_iterator(dirPath))
        {
            if (entry.is_regular_file())
            {
                if (extension.empty() || entry.path().extension() == extension)
                {
                    files.push_back(entry.path());
                }
            }
        }
        return files;
    }

    unsigned int EngineFileIO::GetFileSize(const std::string &path)
    {
        auto fullPath = FindResourcePath(path);
        if (!fullPath.has_value() || !std::filesystem::exists(fullPath.value()))
        {
            Logger::Error("File does not exist: {}", fullPath.value().string());
            return 0;
        }

        return static_cast<unsigned int>(std::filesystem::file_size(fullPath.value()));
    }

} // namespace framework
