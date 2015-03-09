#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "RpcServer.pb.h"

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/Dispatcher.h"
#include "../ServerLib/TcpConnection.h"

class RpcProxyServer
{
public:
    RpcProxyServer();

public:
    void                Run(const std::string& ip, const std::string& port, int pool_size);

private:
    FuncOnAccept        GetAcceptCallBack();
    ConnectionCallBacks GetCallBacks();

    template <typename T>
    void                Send(const ConnectionPtr& conn, const T& t);

private:
    void                HandleLoginRequest(const ConnectionPtr& conn, const RpcServer::LoginRequest& request);
    void                HandleRpcRequest(const ConnectionPtr& conn, const RpcServer::RpcRequest& request);
    void                HandleRpcResponse(const ConnectionPtr& conn, const RpcServer::RpcResponse& response);

private:
    // handler
    void                Dispatch(const ConnectionPtr& conn, const std::string& buf);

    // call backs
    void                OnAccept(const ConnectionPtr& conn);

    void                OnConnected(const ConnectionPtr& conn, bool success);
    int32               OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len);
    void                OnWrite(const ConnectionPtr& conn, int32 len);
    void                OnDisconnect(const ConnectionPtr& conn);

private:
    void                HandlePacket(const ConnectionPtr& conn, const ProtobufMessage& packet);

private:
    PacketDispatcher<ConnectionPtr, ProtobufMessage>    dispatcher_;

    std::mutex                                          mutex_;
    std::unordered_map<std::string, ConnectionPtr>      conn_map_;
};

inline FuncOnAccept RpcProxyServer::GetAcceptCallBack()
{
    return std::bind(&RpcProxyServer::OnAccept, this, std::placeholders::_1);
}

inline ConnectionCallBacks RpcProxyServer::GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&RpcProxyServer::OnConnected, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&RpcProxyServer::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&RpcProxyServer::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&RpcProxyServer::OnDisconnect, this, std::placeholders::_1),
    };
    return cb;
}

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
