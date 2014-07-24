#include "TcpServer.h"

#include "TcpConnection.h"
#include "logger.hpp"

std::shared_ptr<TcpServer> server;

TcpServer::TcpServer(const std::string& address, const std::string& port,
    std::size_t io_service_pool_size, const FuncOnAccept& func)
    : io_service_pool_(io_service_pool_size),
      signals_(io_service_pool_.get_io_service()),
      acceptor_(io_service_pool_.get_io_service()),
      OnAccept(func),
      conn_id_(0),
      new_log_file_timer_(io_service_pool_.get_io_service())
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);

#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

    signals_.async_wait(std::bind([&]() {
        io_service_pool_.stop();
    }));

    logger_.reset(new services::logger(io_service_pool_.get_io_service(), ""));
    logger_->use_file();
    ResetLoggerTimer();

    boost::asio::ip::tcp::resolver resolver(acceptor_.get_io_service());
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    StartAccept();
}

void TcpServer::SetCallBacks(const ConnectionCallBacks& cb)
{
    cb_ = cb;
}

void TcpServer::Run()
{
    io_service_pool_.run();
}

void TcpServer::StartAccept()
{
    ConnectionPtr new_connection(new TcpConnection(io_service_pool_.get_io_service()));
    new_connection->Init(++conn_id_);

    auto handle_accept = [&](ConnectionPtr conn, const ErrorCode& e) {
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

void TcpServer::ResetLoggerTimer()
{
    auto check_new_log_file = [&](boost::asio::deadline_timer * /*deadline*/) {
        logger_->use_file();
        ResetLoggerTimer();
    };

    new_log_file_timer_.expires_from_now(boost::posix_time::hours(NEW_LOG_FILE_INTERVAL));
    new_log_file_timer_.async_wait(std::bind(check_new_log_file, &new_log_file_timer_));
}
