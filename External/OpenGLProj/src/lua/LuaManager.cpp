//
// Created by zhengshulin on 2024/5/21.
//

#include "LuaManager.h"
#include "lua.hpp"
#include <filesystem>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include "../platform/PlatformMacro.h"


static const std::string GAME_INIT ="game_init";
static const std::string GAME_UPDATE ="game_update";
static const std::string GAME_TOUCH ="game_touch";
static const std::string GAME_MESSAGE ="game_message";
static const std::string GAME_PAUSE ="game_pause";

//static const int init_func=1;
static const int update_func=1;
static const int top_func=1;

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

void
fault(const char * format, ...) {

        va_list ap;
                va_start(ap, format);
        pf_vprint(format, ap);
                va_end(ap);
        exit(1);
}
static int
linject(lua_State *L) {
    static const std::string callback[] = {
            GAME_INIT  ,
            GAME_UPDATE,
            GAME_TOUCH ,
            GAME_MESSAGE,
            GAME_PAUSE,
    };
    int i;
    for (i=0;i<sizeof(callback)/sizeof(callback[0]);i++) {
        lua_getfield(L, lua_upvalueindex(1), callback[i].c_str());
//        if (!lua_isfunction(L,-1)) {
//            return luaL_error(L, "%s is not found", callback[i].c_str());
//        }
        if (lua_isfunction(L,-1)) {
            lua_setfield(L, LUA_REGISTRYINDEX, callback[i].c_str());
        }
    }
    return 0;
}


static int framework_c(lua_State *L){
    luaL_Reg l[] = {
            {"inject", linject},
            {NULL,NULL},
    };
    luaL_newlibtable(L, l);
    lua_pushvalue(L,-1);
    luaL_setfuncs(L,l,1);
    return 1;
}

void LuaManager::InitFramework(){
    luaL_requiref(L, "framework.c", framework_c, 0);
    lua_settop(L,0);
}

static void GameStart(){

    lua_getfield(L, LUA_REGISTRYINDEX, GAME_INIT.c_str());
    lua_call(L, 0, 0);
    assert(lua_gettop(L) == 0);
    lua_getfield(L,LUA_REGISTRYINDEX, GAME_UPDATE.c_str());
}

void LuaManager::Update(float time) {
    lua_pushvalue(L,update_func);
    lua_pcall(L,0,0,0);
    lua_settop(L, update_func);
}

void LuaManager::InitLua(char*path){
    if (nullptr== nullptr){
        L = luaL_newstate();
        luaL_openlibs(L);
        luaL_dostring(L,"print('lua init finish!')  print(\"Lua version: \" .. _VERSION)");
    }

    InitFramework();
    std::filesystem::path exePath= std::filesystem::path(path);
    std::filesystem::path dir=exePath.parent_path();
    std::filesystem::path scriptRoot=dir / "res"/ "lua";


    auto script = R"(
        local path, script = ...
        assert(script, 'I need a script name')
        package.path = path .. [[/?.lua;]] .. path .. [[/?;]] .. path .. [[/?/init.lua;./?.lua;./?/init.lua]]
        local f = assert(loadfile(script))
        f(script)
    )";

    int err=luaL_loadstring(L, script);
    if (err) {
        const char *msg = lua_tostring(L,-1);
        fault("%s", msg);
    }
    lua_pushlstring(L, scriptRoot.string().c_str(),scriptRoot.string().size());
    lua_pushstring(L, "res/lua/example/ex01.lua");

    err = lua_pcall(L, 2, 0, 0);
    if (err) {
        const char *msg = lua_tostring(L,-1);
        fault("%s", msg);
    }

    StackDump();


    GameStart();
}

