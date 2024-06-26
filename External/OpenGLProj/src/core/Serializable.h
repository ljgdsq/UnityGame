//
// Created by zhengshulin on 2024/6/26.
//

#ifndef OPENGLPROJ_SERIALIZABLE_H
#define OPENGLPROJ_SERIALIZABLE_H

#endif //OPENGLPROJ_SERIALIZABLE_H
#include <fstream>

class Serializable {
public:
    virtual void serialize(std::ostream& os)  =0;
    virtual void deserialize(std::istream& is) = 0;
};