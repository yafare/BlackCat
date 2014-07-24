#ifndef _SOCKET_H
#define _SOCKET_H

#include "ServerCommon.h"

class Socket
{
public:
    Socket(IoService& io_service);
    ~Socket();

public:
    TcpSocket& socket();
    uint8 *GetBuf();

public:
    bool IsOpen() const;
    void Shutdown();

public:
    void Connect(const std::function<void(const ErrorCode&)>& cb, const std::string& ip, uint16 port);
    void Send(const std::function<void(const ErrorCode&, uint32)>& cb, const uint8 *data, uint32 size);
    void Recv(const std::function<void(const ErrorCode&, uint32)>& cb);

private:
    IoService                           io_service_;
    TcpSocket                           socket_;
    boost::array<uint8, MAX_RECV_BUF>   buffer_;
};

inline TcpSocket& Socket::socket()
{
    return socket_;
}

inline uint8 *Socket::GetBuf()
{
    return buffer_.data();
}

inline bool Socket::IsOpen() const
{
    return socket_.is_open();
}

inline void Socket::Shutdown()
{
    // NO EXCEPTION
    ErrorCode ec;
    socket_.shutdown(boost::asio::socket_base::shutdown_both, ec);
}

#endif // _SOCKET_H
