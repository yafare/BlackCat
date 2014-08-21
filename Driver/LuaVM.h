#ifndef _LUAVM_H
#define _LUAVM_H

#include <string>
#include <vector>

#include <luabind/luabind.hpp>

#include "../ServerLib/logger.hpp"

struct lua_State;
class LuaVM
{
public:
    LuaVM();
    ~LuaVM();

public:
    lua_State      *GetLuaState();

    void            Execute(const std::string& script);
    bool            Load(const std::string& file);

    template <typename... Args>
    void            Call(const std::string& func, Args&... args);

    template <typename TRet, typename... Args>
    TRet            Call(const std::string& func, Args&... args);

private:
    lua_State      *L;
};
inline lua_State *LuaVM::GetLuaState()
{
    return L;
}
template <typename... Args>
void LuaVM::Call(const std::string& func, Args&... args)
{
    if (L == 0) {
        return;
    }
    try {
        luabind::call_function<void>(L, func.c_str(), args...);
    } catch (...) {
        LOG("exception @ lua function: %s", func.c_str());
    }
}
template <typename TRet, typename... Args>
TRet LuaVM::Call(const std::string& func, Args&... args)
{
    if (L == 0) {
        return TRet();
    }
    try {
        return luabind::call_function<TRet>(L, func.c_str(), args...);
    } catch (...) {
        LOG("exception @ lua function: %s", func.c_str());
    }
    return TRet();
}

#endif  // _LUAVM_H
