//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_FILEUTIL_H
#define OPENGLPROJ_FILEUTIL_H

#include <string>
#include <filesystem>
namespace fs=std::filesystem;
class FileUtil {
public:
    static std::string NormalizePath(const std::filesystem::path& path){
        std::string normalized_path = path.generic_string();
        for (char& c : normalized_path) {
            if (c == '\\') {
                c = '/';
            }
        }
        return normalized_path;
    }
    static std::string NormalizePath(const std::string&path){
        std::filesystem::path fs_path(path);
//        std::string normalizedPath = path.string();
//#ifdef _WIN32
//        std::replace(normalizedPath.begin(), normalizedPath.end(), '/', '\\');
//#else
//        std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
//#endif

        return NormalizePath(fs_path);
    }

    static bool FileExists(const std::string& fullPath){
        return fs::exists(fullPath);
    }

    static bool PathExists(const std::string& fullPath){
        return fs::exists(fullPath);
    }

    static bool RemoveFile(const std::string& fullPath){
        if (FileExists(fullPath)){
            return fs::remove(fullPath);
        }
        return false;
    }

    static bool WriteText(const std::string& fullPath,const std::string& text);
};


#endif //OPENGLPROJ_FILEUTIL_H
