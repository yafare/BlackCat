#include <stdio.h>

#include <luabind/luabind.hpp>

#include "../ServerLib/TcpClient.h"
#include "../Common/Types.h"
#include "LuaVM.h"

using namespace luabind;

void TestLua()
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
}

void OnConnected(const ConnectionPtr& conn)
{
    conn->Send((const uint8 *)"fuck", 4);
    printf("OnConnected\n");
}
uint32 OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
{
    printf("%s\n", buf);
    return len;
}
void OnWrite(const ConnectionPtr& conn, uint32 len)
{
    printf("%d\n", len);
}
void OnDisconnect(const ConnectionPtr& conn)
{
}

void TestTcpClient()
{
    IoService ios;
    TcpClientPtr client(new TcpClient(ios));

    ConnectionCallBacks cb = {
        std::bind(&OnConnected, std::placeholders::_1),
        std::bind(&OnRead, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&OnWrite, std::placeholders::_1, std::placeholders::_2),
        std::bind(&OnDisconnect, std::placeholders::_1),
    };
    client->SetCallBacks(cb);
    client->Connect("218.104.71.174", "23");

    ios.run();
}

int main()
{
    TestTcpClient();
    getchar();
}
