#include "TcpClient.h"

TcpClient::TcpClient(IoService& service) : TcpConnection(service, [](uint32){}), connected_(false)
{
}

void TcpClient::Connect(const std::string& ip, const std::string& port)
{
    Resolver resolver(io_service_);
    Resolver::query query(ip, port);
    Resolver::iterator endpoint_it = resolver.resolve(query);

    auto handle_connected = [&](const ConnectionPtr& conn, const ErrorCode& e) {
        if (!e) {
            connected_ = true;
            Recv();
        }
        conn->OnConnected(e);
    };
    boost::asio::async_connect(GetSocket().socket(), endpoint_it,
        std::bind(handle_connected, shared_from_this(), std::placeholders::_1));
}
