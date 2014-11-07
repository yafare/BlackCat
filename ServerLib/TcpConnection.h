#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "ServerCommon.h"
#include "Socket.h"

#define INVALID_CONN_ID -1

class TcpConnection
    : private boost::noncopyable,
      public std::enable_shared_from_this<TcpConnection>
{
    using DisconnectCallBack = std::function<void(uint32)>;
    using BufferPtr = std::shared_ptr<std::vector<uint8>>;
public:
    TcpConnection(IoService& io_service, DisconnectCallBack disconnect_func);
    ~TcpConnection();

    void            Init(uint32 id);

public:
    uint32          GetId();
    Socket&         GetSocket();
    IoService&      GetIoService();

public:
    void            SetCallBacks(const ConnectionCallBacks& callbacks);
    void            Send(const uint8 *buf, int32 len);
    void            Recv();
    void            Shutdown();

private:
    void            RealSend(const BufferPtr& buffer);

public:
    void            OnConnected(const ErrorCode& e);
    void            OnRead(const ErrorCode& e, int32 len);
    void            OnWrite(const ErrorCode& e, int32 len);
    void            OnDisconnect(const ErrorCode& e);

protected:
    uint32                      conn_id_;
    IoService&                  io_service_;
    Socket                      socket_;
    ConnectionCallBacks         cb_;
    bool                        writting_msg_;
    std::deque<BufferPtr>       send_buf_;
    std::vector<uint8>          recv_buf_;
    DisconnectCallBack          disconnect_func_;
};

inline uint32 TcpConnection::GetId()
{
    return conn_id_;
}

inline Socket& TcpConnection::GetSocket()
{
    return socket_;
}
inline IoService& TcpConnection::GetIoService()
{
    return io_service_;
}
inline void TcpConnection::SetCallBacks(const ConnectionCallBacks& callbacks)
{
    cb_ = callbacks;
}

inline void TcpConnection::Shutdown()
{
    socket_.Shutdown();
}

#endif // _TCPCONNECTION_H
