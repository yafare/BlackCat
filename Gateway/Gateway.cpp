#include "Gateway.h"

#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"

#include "Packet.pb.h"
#include "Gateway.pb.h"

#include "GateUser.h"

Gateway::Gateway()
{
}

void Gateway::Run(const GatewayStartupConfig& config)
{
    InitRpcService(config.service_name, config.rpc_server_addr);

    GenericServer::Run(config.ip, config.port, config.pool_size);
}

void Gateway::InitRpcHandler()
{
    gateway_->Register<GatewayServer::ReportAvailable>([this](const GatewayServer::ReportAvailable& req) -> rpclib::RpcResult {
        const std::string& serv_name = req.server_name();
        for (int i = 0; i < req.protocol_size(); i++) {
            protocol_handlers_[req.protocol(i)].insert(serv_name);
        }

        GatewayServer::ReportAvailableResponse response;
        response.set_result(1);
        return rpclib::MakeRpcResult(response);
    });
}
void Gateway::InitRpcService(const std::string& name, const std::string& rpc_server)
{
    gateway_.reset(new rpclib::RpcServiceProvider(name));
    {
        InitRpcHandler();
    }
    gateway_->Connect(rpc_server);
}

void Gateway::Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len)
{
    PB::Packet packet;
    if (!packet.ParseFromArray(buf, len)) {
        return;
    }

    HandlePacket(conn, packet);
}

void Gateway::OnAccept(const ConnectionPtr& conn)
{
    GetGateUserManager().AddUser(conn);
}

void Gateway::OnDisconnect(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetGateUserManager().RemoveUser(conn_id);
}
