#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include "ServerCommon.h"
#include "TcpConnection.h"

class TcpClient : public TcpConnection
{
public:
    TcpClient(IoService& service);

public:
    void            Connect(const std::string& ip, const std::string& port);
    bool            Connected();

private:
    bool            connected_;
    EndPoint        endpoint_;
};
using TcpClientPtr = std::shared_ptr<TcpClient>;

inline bool TcpClient::Connected()
{
    return connected_;
}

#endif // _TCPCLIENT_H
