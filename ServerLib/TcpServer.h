#ifndef _SERVER_H
#define _SERVER_H

#include <atomic>

#include "ServerCommon.h"
#include "io_service_pool.hpp"

#define NEW_LOG_FILE_INTERVAL 2 //hours

class TcpServer : private boost::noncopyable
{
public:
    explicit TcpServer(const std::string& address, const std::string& port,
        std::size_t io_service_pool_size, const FuncOnAccept& func);

    void SetCallBacks(const ConnectionCallBacks& cb);

    void Run();
    void Log(const std::string& str);
    void Log(const char *fmt, ...);

private:
    void StartAccept();

    void ResetLoggerTimer();

private:
    io_service_pool             io_service_pool_;
    boost::asio::signal_set     signals_;
    Acceptor                    acceptor_;
    FuncOnAccept                OnAccept;
    ConnectionCallBacks         cb_;

    std::atomic<int32>          conn_id_;

    // logger timer
    boost::asio::deadline_timer new_log_file_timer_;
};
extern std::shared_ptr<TcpServer> server;

#endif // _SERVER_H
