#include "TcpServer.h"

#include "TcpConnection.h"
#include "logger.hpp"

std::shared_ptr<TcpServer> server;

TcpServer::TcpServer(const std::string& address, const std::string& port,
    std::size_t io_service_pool_size, const FuncOnAccept& func)
    : io_service_pool_(io_service_pool_size),
      signals_(io_service_pool_.get_io_service()),
      acceptor_(io_service_pool_.get_io_service()),
      OnAccept(func)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);

#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

    signals_.async_wait(std::bind([&]() {
        io_service_pool_.stop();
    }));

    boost::asio::ip::tcp::resolver resolver(acceptor_.get_io_service());
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    StartAccept();
}

void TcpServer::StartAccept()
{
    ConnectionPtr new_connection(new TcpConnection(io_service_pool_.get_io_service(),
        std::bind(&TcpServer::ConnectionDown, this, std::placeholders::_1)));
    new_connection->Init(conn_id_mgr_.GetId());

    auto handle_accept = [&](const ConnectionPtr& conn, const ErrorCode& e) {
        if (!e) {
            if (OnAccept) {
                OnAccept(conn);
            }
            conn->SetCallBacks(cb_);
            conn->Recv();
        }
        StartAccept();
    };
    acceptor_.async_accept(new_connection->GetSocket().socket(),
        std::bind(handle_accept, new_connection, std::placeholders::_1));
}
