#include <stdio.h>

#include "DBService.pb.h"

#include "../RpcLib/rpc_service.h"
#include "../ServerLib/Dispatcher.h"

const char *db_name = "db";
const char *gate0_name = "gate0";

int main()
{
    rpclib::RpcServiceProvider db(db_name);
    rpclib::RpcServiceProvider gate0(gate0_name);

    db.Connect("127.0.0.1:10001");
    gate0.Connect("127.0.0.1:10001");

    Sleep(2000);

    {
        db.Register<DBService::EchoRequest>([](const DBService::EchoRequest& req) -> rpclib::RpcResult {
            DBService::EchoResponse response;
            response.set_msg(req.msg());
            return rpclib::MakeRpcResult(response);
        });
    }

    {
        DBService::EchoRequest req;
        req.set_msg("Hello, world!\n");
        gate0.Call<DBService::EchoRequest>(db_name, req, [](const rpclib::RpcResult& rsp) {
            printf("result: %s\n", rsp.name);

            auto message = CreateMessage(rsp.name);
            if (message != 0) {
                message->ParseFromString(rsp.body);
                DBService::EchoResponse *m = dynamic_cast<DBService::EchoResponse *>(message);
                if (m) {
                    printf("\t%s\n", m->msg().c_str());
                }
                delete message;
            }
        });
    }

    getchar();
}
