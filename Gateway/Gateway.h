#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "../ServerLib/GenericServer.h"
#include "../RpcLib/rpc_service.h"

#include <unordered_set>
#include <unordered_map>

namespace PB
{
    class Packet;
}

struct GatewayStartupConfig
{
    std::string     service_name;
    std::string     rpc_server_addr;

    std::string     ip;
    std::string     port;
    int             pool_size;
};

class Gateway : public GenericServer
{
public:
    Gateway();

public:
    void            Run(const GatewayStartupConfig& info);

private:
    void            InitRpcService(const std::string& name, const std::string& rpc_server);
    void            InitRpcHandler();

private:
    // handler
    virtual void    OnAccept(const ConnectionPtr& conn);
    virtual void    OnDisconnect(const ConnectionPtr& conn);
    virtual void    Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len);

private:
    void            HandlePacket(const ConnectionPtr& conn, const PB::Packet& packet);

private:
    rpclib::RpcServiceProviderPtr gateway_;
    std::unordered_map<std::string, std::unordered_set<std::string>> protocol_handlers_;
};

#endif // _GATEWAY_H
