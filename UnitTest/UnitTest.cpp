#include <stdio.h>

#include <luabind/luabind.hpp>

#include "../Common/Types.h"
#include "LuaVM.h"

using namespace luabind;

int main()
{
    LuaVM *lua_vm = new LuaVM;

    // BaseLuaTest
    {
        lua_vm->Load("test.lua");

        try {
            uint64 id = 123;
            uint64 v = lua_vm->Call<uint64, uint64>("Test", id);
            printf("%lld\n", v);

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
        } catch (...) {
            printf("exception @ BaseLuaTest\n");
        }
    }

    // pbc Test
    {
        lua_vm->Load("test2.lua");
    }

    // LuaPBTest
    {
        lua_vm->Load("testluapb.lua");

        try {
            lua_vm->Call("encodeSimp");
        } catch (...) {
            printf("exception @ encodeSimp\n");
        }

        lua_vm->Call("test");
    }

    delete lua_vm;
    getchar();
}
