#include "LuaBinder.h"

#include <luabind/luabind.hpp>

#include "LuaVM.h"
#include "Driver.h"

Driver *GetDriver()
{
    return driver;
}

void LuaBinder::Bind(LuaVM *vm)
{
    using namespace luabind;
    module(vm->GetLuaState())
    [
        def("GetDriver", &GetDriver),
        class_<Driver>("Driver")
            .def("Send", &Driver::Send)
    ];
}
