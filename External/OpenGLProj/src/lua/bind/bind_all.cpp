//
// Created by zhengshulin on 2024/5/31.
//

#include "bind_all.h"
#include "../../ui/Sprite.h"

static int
create(lua_State *L) {
    const char * name = lua_tostring(L,-1);
    auto sp=Sprite::Create(name);
    lua_pushlightuserdata(L,sp);
    return 1;
}


static int sprite_pos(lua_State *L) {
    Sprite* sp = static_cast<Sprite*>(lua_touserdata(L, 1));
    if (sp) {
        int x=lua_tonumber(L,-1);
        int y=lua_tonumber(L,-2);
        sp->Position(x,y);
    }
    return 0;
}

static int sprite_size(lua_State *L) {
//    Sprite* sp = static_cast<Sprite*>(lua_touserdata(L, 1));
//    if (sp) {
//        Vec2 size = sp->size();
//        pushVec2(L, size);
//    } else {
//        lua_pushnil(L);
//    }
    return 0;
}

static int sprite_draw(lua_State *L) {
    Sprite* sp = static_cast<Sprite*>(lua_touserdata(L, 1));
    sp->Draw();
    return 0;
}
static int
game_sprite(lua_State *L) {
luaL_Reg l[] = {
        {"new", create},
        {"pos", sprite_pos},
        {"size", sprite_size},
        {"draw", sprite_draw},
        {NULL,NULL},
};
luaL_newlib(L,l);
return 1;
}





void game_bind_all(lua_State *L) {
    luaL_requiref(L, "game.sprite", game_sprite, 0);
//    luaL_requiref(L, "test.obj", test_obj, 0);
    lua_settop(L,0);
}
