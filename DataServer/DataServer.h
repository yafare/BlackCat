#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "../ServerLib/ServerCommon.h"

namespace PB
{
    class Packet;
}

class DataServer
{
public:
    DataServer();

public:
    void                Run(const std::string& ip, const std::string& port, int pool_size = 1);

private:
    FuncOnAccept        GetAcceptCallBack();
    ConnectionCallBacks GetCallBacks();

private:
    // handler
    void                Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len);

    // call backs
    void                OnAccept(const ConnectionPtr& conn);

    void                OnConnected(const ConnectionPtr& conn, bool success);
    int32               OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len);
    void                OnWrite(const ConnectionPtr& conn, int32 len);
    void                OnDisconnect(const ConnectionPtr& conn);
};

inline FuncOnAccept DataServer::GetAcceptCallBack()
{
    return std::bind(&DataServer::OnAccept, this, std::placeholders::_1);
}

inline ConnectionCallBacks DataServer::GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&DataServer::OnConnected, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&DataServer::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&DataServer::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&DataServer::OnDisconnect, this, std::placeholders::_1),
    };
    return cb;
}

#endif // _GATEWAY_H
