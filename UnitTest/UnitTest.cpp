#include <stdio.h>

#include "../ServerLib/lua_tinker.h"

#pragma comment(lib, "lua52.lib")

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_tinker::init(L);

    lua_tinker::dofile(L, "./script/test.lua");
    __int64 id = 0x1234567887654321;
    lua_tinker::call<void, __int64>(L, "TestI64", id);

    lua_tinker::table fishes(L);
    int index = 1;
    for (int i = 1; i <= 10; i++) {
        lua_tinker::table fish(L);
        fish.set("fish_type", i);
        fish.set("fish_count", i);
        fishes.set(index++, fish);
    }
    lua_tinker::table result = lua_tinker::call<lua_tinker::table, __int64, lua_tinker::table>(L, "TestTable", id, fishes);

    getchar();
}
