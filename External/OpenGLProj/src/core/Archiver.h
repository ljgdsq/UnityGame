//
// Created by zhengshulin on 2024/6/26.
//

#ifndef OPENGLPROJ_ARCHIVER_H
#define OPENGLPROJ_ARCHIVER_H

#include <string>

template<class T>
class Archiver {
public:

    std::string save(const T*obj){
        return "";
    }

    void load(const T*obj,std::string&data){

    };
};


#endif //OPENGLPROJ_ARCHIVER_H
