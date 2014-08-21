#include "LuaVM.h"

extern "C"
{
#include <lua.hpp>
};

#include <luabind/operator.hpp>

#include "../Common/Types.h"

LuaVM::LuaVM()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    luabind::open(L);

#define POD_MODULE(t) \
    luabind::class_<t>(#t) \
        .def(luabind::constructor<t>()) \
        .def(luabind::constructor<int>()) \
        .def(luabind::tostring(luabind::self)) \
        .def(luabind::const_self * int()) \
        .def(luabind::const_self * t()) \
        .def(luabind::const_self * luabind::const_self) \
        .def(luabind::const_self / int()) \
        .def(luabind::const_self / t()) \
        .def(luabind::const_self / luabind::const_self) \
        .def(luabind::const_self + int()) \
        .def(luabind::const_self + t()) \
        .def(luabind::const_self + luabind::const_self) \
        .def(luabind::const_self - int()) \
        .def(luabind::const_self - t()) \
        .def(luabind::const_self - luabind::const_self) \
        .def(luabind::const_self < t()) \
        .def(luabind::const_self == t()) \
        .def(luabind::const_self <= t())

    luabind::module(L)
    [
        POD_MODULE(int64),
        POD_MODULE(uint64)
    ];
}

LuaVM::~LuaVM()
{
    if (L) {
        lua_close(L);
        L = 0;
    }
}

void LuaVM::Execute(const std::string& script)
{
    if (L) {
        luaL_dostring(L, script.c_str());
    }
}

bool LuaVM::Load(const std::string& file)
{
    if (L) {
        return luaL_dofile(L, file.c_str()) == 0;
    }
    return false;
}
