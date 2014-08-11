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
      new_log_file_timer_(io_service_pool_.get_io_service()),
      server_logic_timer_(io_service_pool_.get_io_service())
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

    // Timers
    ResetLogicTimer();

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

void TcpServer::ResetLogicTimer()
{
    auto logic_timer_func = [&]() {
        {
            Lock l(timer_mutex_);
            for (auto it = timer_map_.begin(); it != timer_map_.end(); it++) {
                boost::timer::cpu_times elapsed_times(it->second.timer.elapsed());
                boost::timer::nanosecond_type elapsed(elapsed_times.wall);
                if (elapsed >= it->second.interval * 1000000LL) {
                    it->second.func();
                    it->second.timer.start();
                }
            }
        }

        ResetLogicTimer();
    };

    server_logic_timer_.expires_from_now(boost::posix_time::milliseconds(LOGIC_TIMER_INTERVAL));
    server_logic_timer_.async_wait(std::bind(logic_timer_func));
}

uint32 TcpServer::SetTimer(const TimerFunc& func, uint32 interval)
{
    uint32 timer_id = timer_id_++;

    TimerContext timer = { interval, func };
    {
        Lock l(timer_mutex_);
        timer_map_[timer_id] = timer;
    }

    return timer_id;
}

void TcpServer::KillTimer(uint32 timer_id)
{
    Lock l(timer_mutex_);
    auto it = timer_map_.find(timer_id);
    if (it == timer_map_.end()) {
        return;
    }
    timer_map_.erase(it);
}

void TcpServer::StartAccept()
{
    ConnectionPtr new_connection(new TcpConnection(io_service_pool_.get_io_service()));
    new_connection->Init(++conn_id_);

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

void TcpServer::ResetLoggerTimer()
{
    auto check_new_log_file = [&](boost::asio::deadline_timer * /*deadline*/) {
        logger_->use_file();
        ResetLoggerTimer();
    };

    new_log_file_timer_.expires_from_now(boost::posix_time::hours(NEW_LOG_FILE_INTERVAL));
    new_log_file_timer_.async_wait(std::bind(check_new_log_file, &new_log_file_timer_));
}
