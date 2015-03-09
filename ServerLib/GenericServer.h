#ifndef _GENERIC_SERVER_H
#define _GENERIC_SERVER_H

#include "../ServerLib/ServerCommon.h"

class GenericServer
{
public:
    GenericServer();

public:
    void                Run(const std::string& ip, const std::string& port, int pool_size = 1);

private:
    FuncOnAccept        GetAcceptCallBack();
    ConnectionCallBacks GetCallBacks();

private:
    // handler
    virtual void        Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len) {}

    // call backs
    virtual void        OnAccept(const ConnectionPtr& conn);

    virtual void        OnConnected(const ConnectionPtr& conn, bool success);
    virtual int32       OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len);
    virtual void        OnWrite(const ConnectionPtr& conn, int32 len);
    virtual void        OnDisconnect(const ConnectionPtr& conn);
};

inline FuncOnAccept GenericServer::GetAcceptCallBack()
{
    return std::bind(&GenericServer::OnAccept, this, std::placeholders::_1);
}

inline ConnectionCallBacks GenericServer::GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&GenericServer::OnConnected, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&GenericServer::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&GenericServer::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&GenericServer::OnDisconnect, this, std::placeholders::_1),
    };
    return cb;
}

#endif // _GENERIC_SERVER_H
