#include <stdio.h>

#include <luabind/luabind.hpp>

#include "../Common/Types.h"
#include "LuaVM.h"

using namespace luabind;

int main()
{
    LuaVM *lua_vm = new LuaVM;

    lua_vm->Load("test.lua");
    module(lua_vm->GetLuaState())
    [
        class_<int64, std::shared_ptr<int64>>("int64")
    ];

    int id = 123;
    lua_vm->Call("Test", id);

    object params(newtable(lua_vm->GetLuaState()));
    for (int i = 1; i <= 10; i++) {
        object param(newtable(lua_vm->GetLuaState()));
        param["type"] = i;
        param["count"] = i;
        params[i] = param;
    }
    object result = lua_vm->Call<object, int, object>("TestTable", id, params);
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
