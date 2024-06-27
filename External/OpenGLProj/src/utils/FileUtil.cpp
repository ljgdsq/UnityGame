//
// Created by zhengshulin on 2024/5/21.
//

#include "FileUtil.h"
#include <fstream>

using std::ofstream;
bool FileUtil::WriteText(const std::string &fullPath, const std::string &text) {
    ofstream outFile(fullPath);
    if (outFile.is_open()){
        outFile << text;
        outFile.close();
        return true;
    }else{
        return false;
    }
}
