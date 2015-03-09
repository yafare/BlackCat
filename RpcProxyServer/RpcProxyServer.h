#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "RpcServer.pb.h"

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/GenericServer.h"
#include "../ServerLib/Dispatcher.h"
#include "../ServerLib/TcpConnection.h"

class RpcProxyServer : public GenericServer
{
public:
    RpcProxyServer();

public:
    template <typename T>
    void            Send(const ConnectionPtr& conn, const T& t);

private:
    void            HandleLoginRequest(const ConnectionPtr& conn, const RpcServer::LoginRequest& request);
    void            HandleRpcRequest(const ConnectionPtr& conn, const RpcServer::RpcRequest& request);
    void            HandleRpcResponse(const ConnectionPtr& conn, const RpcServer::RpcResponse& response);

private:
    // handler
    virtual int32   OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len);
    virtual void    OnDisconnect(const ConnectionPtr& conn);

private:
    void            Dispatch(const ConnectionPtr& conn, const std::string& buf);
    void            HandlePacket(const ConnectionPtr& conn, const ProtobufMessage& packet);

private:
    PacketDispatcher<ConnectionPtr, ProtobufMessage>    dispatcher_;

    std::mutex                                          mutex_;
    std::unordered_map<std::string, ConnectionPtr>      conn_map_;
};

template <typename T>
void RpcProxyServer::Send(const ConnectionPtr& conn, const T& t)
{
    auto pkt = Encode(t);
    if (pkt.empty()) {
        // TODO
        return;
    }
    conn->Send(reinterpret_cast<const uint8 *>(pkt.c_str()), static_cast<int32>(pkt.size()));
}

#endif // _GATEWAY_H
