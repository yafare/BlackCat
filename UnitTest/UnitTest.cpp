#include <stdio.h>

#include <luabind/luabind.hpp>

#include "../Common/Types.h"
#include "LuaVM.h"

using namespace luabind;

int main()
{
    for (int i = 0; i < 1000; i++) {
        LuaVM *lua_vm = new LuaVM;
        lua_vm->Load("test2.lua");
    }


    LuaVM *lua_vm = new LuaVM;

    // pbc Test
    {
        lua_vm->Load("test2.lua");
    }

    delete lua_vm;
    getchar();
}
