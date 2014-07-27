#include <stdio.h>

#include <luabind/luabind.hpp>

#include "../Common/Types.h"
#include "LuaVM.h"

using namespace luabind;

int main()
{
    LuaVM *lua_vm = new LuaVM;

    lua_vm->Load("test.lua");

    uint64 id = 123;
    uint64 v = lua_vm->Call<uint64, uint64>("Test", id);

    object params(newtable(lua_vm->GetLuaState()));
    for (int i = 1; i <= 10; i++) {
        object param(newtable(lua_vm->GetLuaState()));
        param["type"] = i;
        param["count"] = i;
        params[i] = param;
    }
    object result = lua_vm->Call<object, uint64, object>("TestTable", id, params);
    for (iterator it(result), end; it != end; it++) {
        std::string key = object_cast<std::string>(it.key());

        object val = *it;
        if (type(val) == LUA_TNUMBER) {
            int v = object_cast<int>(val);
            printf("CPP: %s %d\n", key.c_str(), v);
        }
    }

    getchar();
}
