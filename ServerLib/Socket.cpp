#include "Socket.h"

Socket::Socket(IoService& io_service) : socket_(io_service)
{
}

Socket::~Socket()
{
}

void Socket::Connect(const std::function<void(const ErrorCode&)>& cb, const std::string& ip, uint16 port)
{
    if (IsOpen()) {
        Shutdown();
    }

    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(ip), port);

    socket_.async_connect(endpoint, cb);
}

void Socket::Send(const std::function<void(const ErrorCode&, uint32)>& cb, const uint8 *data, uint32 size)
{
    if (data == 0 || size == 0) {
        return;
    }
    boost::asio::async_write(socket_, boost::asio::buffer(data, size), cb);
}

void Socket::Recv(const std::function<void(const ErrorCode&, uint32)>& cb)
{
    socket_.async_read_some(boost::asio::buffer(buffer_), cb);
}
