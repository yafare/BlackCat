#include "Gateway.h"

#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"

#include "packet.pb.h"

#include "GateUser.h"

Gateway::Gateway()
{
}

void Gateway::Run(const GatewayStartupConfig& config)
{
    InitRpcService(config.service_name, config.rpc_server_addr);

    GenericServer::Run(config.ip, config.port, config.pool_size);
}

void Gateway::InitRpcService(const std::string& name, const std::string& rpc_server)
{
    gateway_.reset(new rpclib::RpcServiceProvider(name));
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
