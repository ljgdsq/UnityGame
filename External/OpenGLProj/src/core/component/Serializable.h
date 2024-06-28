//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_SERIALIZABLE_H
#define OPENGLPROJ_SERIALIZABLE_H
#include "rapidjson/document.h"

class Serializable {
public:
    virtual rapidjson::Value serialize()=0;
    virtual void deserialize()=0;
};


#endif //OPENGLPROJ_SERIALIZABLE_H
