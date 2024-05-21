//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_LUAMANAGER_H
#define OPENGLPROJ_LUAMANAGER_H


class LuaManager {
public:
    static LuaManager*GetInstance();
    void InitLua(char*path);
    void InitFramework();
    void StackDump();
    void Update(float time);
private:
};


#endif //OPENGLPROJ_LUAMANAGER_H
