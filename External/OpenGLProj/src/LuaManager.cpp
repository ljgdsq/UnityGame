//
// Created by zhengshulin on 2024/5/21.
//

#include "LuaManager.h"
#include "lua.hpp"
#include "res_path.h"
#include <filesystem>
static lua_State*L= nullptr;
LuaManager* LuaManager::GetInstance(){
    static LuaManager manager;
    return &manager;
}

void LuaManager::StackDump() {
    if (L== nullptr)return;
    int top = lua_gettop(L);
    printf("Lua stack has %d elements:\n", top);
    for (int i = 1; i <= top; i++) { // Lua stack indices start at 1
        int type = lua_type(L, i);
        switch (type) {
            case LUA_TSTRING:
                printf("%d: %s (string)\n", i, lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%d: %s (boolean)\n", i, lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("%d: %g (number)\n", i, lua_tonumber(L, i));
                break;
            default:
                printf("%d: %s (%s)\n", i, lua_typename(L, type), luaL_tolstring(L, i, NULL));
                lua_pop(L, 1); // Pop the string created by luaL_tolstring
                break;
        }
    }
}


void LuaManager::InitLua(char*path){
    if (nullptr== nullptr){
        L = luaL_newstate();
        luaL_openlibs(L);
    }
    luaL_dostring(L,"print('lua init finish!')  print(\"Lua version: \" .. _VERSION)");
    luaL_dostring(L,"package.path = path .. [[\\?.lua;]] .. path .. [[\\?\\init.lua;.\\?.lua;.\\?\\init.lua]]\n");
    StackDump();
    std::filesystem::path resPath =std::string( LUA_SCRIPT_PATH) + "/init.lua";

    luaL_dofile(L, reinterpret_cast<const char *>(resPath.c_str()));
}