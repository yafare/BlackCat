#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "ServerCommon.h"
#include "Socket.h"

class TcpConnection
    : private boost::noncopyable,
      public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(IoService& io_service);
    ~TcpConnection();

    void            Init(uint32 id);

public:
    uint32          GetId();
    Socket&         GetSocket();

public:
    void            SetCallBacks(const ConnectionCallBacks& callbacks);
    void            Send(const uint8 *buf, uint32 len);
    void            Recv();

private:
    void            OnConnected(const ErrorCode& e);
    void            OnRead(const ErrorCode& e, uint32 len);
    void            OnWrite(const ErrorCode& e, uint32 len);
    void            OnDisconnect(const ErrorCode& e);

private:
    uint32                      conn_id_;
    Socket                      socket_;
    ConnectionCallBacks         cb_;
    std::vector<uint8>          recv_buf_;
    std::mutex                  mutex_;
};

inline uint32 TcpConnection::GetId()
{
    return conn_id_;
}

inline Socket& TcpConnection::GetSocket()
{
    return socket_;
}
inline void TcpConnection::SetCallBacks(const ConnectionCallBacks& callbacks)
{
    cb_ = callbacks;
}

#endif // _TCPCONNECTION_H
