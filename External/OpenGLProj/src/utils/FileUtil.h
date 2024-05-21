//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_FILEUTIL_H
#define OPENGLPROJ_FILEUTIL_H

#include <string>
#include <filesystem>
class FileUtil {
public:
    static std::string NormalizePath(const std::string&path){
        std::filesystem::path fs_path(path);

        std::string normalized_path = fs_path.generic_string();
        for (char& c : normalized_path) {
            if (c == '\\') {
                c = '/';
            }
        }
        return normalized_path;
    }
};


#endif //OPENGLPROJ_FILEUTIL_H
