#include "../ServerLib/ServerCommon.h"

#include <stdio.h>

#include <thread>

#include <Windows.h>

#include "../RpcLib/rpc_task.h"
#include "../RpcLib/rpc_service.h"

#include "RpcServer.pb.h"

void TestRpcTask()
{
    rpclib::RpcTaskMgr task_mgr;
    std::thread thread(std::bind(&rpclib::RpcTaskMgr::Run, &task_mgr));
    thread.detach();

    task_mgr.Push(rpclib::RpcTask([](){
            printf("Hello, world 1\n");
            Sleep(3000);
        })
    );
    task_mgr.Push(rpclib::RpcTask([](){
            printf("Hello, world 2\n");
        })
    );

    Sleep(1000);

    printf("Stop now\n");
    task_mgr.Stop();

    task_mgr.Push(rpclib::RpcTask([](){
            printf("Hello, world 3\n");
        })
    );

    printf("Stopped\n");
}

void TestProtobufCodec()
{
    RpcServer::LoginRequest req;
    req.set_service_name("@echo");
    req.set_secure_key("test key");

    auto r = Encode(req);

    std::string d;
    std::copy(r.begin() + 4, r.end(), std::back_inserter(d));
    auto p = Decode(d);

    printf("%s\n", p->GetTypeName());
}

void Dispatch(const RpcServer::LoginRequest& req)
{
    printf("dispatch RpcServer::LoginRequest\n");
}

int main()
{
    getchar();
}
