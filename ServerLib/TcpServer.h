#ifndef _SERVER_H
#define _SERVER_H

#include <atomic>
#include <boost/timer/timer.hpp>

#include "ServerCommon.h"
#include "io_service_pool.hpp"

#define NEW_LOG_FILE_INTERVAL 2 // hours
#define LOGIC_TIMER_INTERVAL  2 // ms

class TcpServer : private boost::noncopyable
{
    struct TimerContext
    {
        uint32 interval;
        TimerFunc func;
        boost::timer::cpu_timer timer;
    };

public:
    explicit TcpServer(const std::string& address, const std::string& port,
        std::size_t io_service_pool_size, const FuncOnAccept& func);

    void    SetCallBacks(const ConnectionCallBacks& cb);

    void    Run();

    // create a timer, interval: ms, return the ID of timer
    uint32  SetTimer(const TimerFunc& func, uint32 interval);
    void    KillTimer(uint32 timer_id);

private:
    void    StartAccept();

    void    ResetLoggerTimer();
    void    ResetLogicTimer();

private:
    io_service_pool                     io_service_pool_;
    boost::asio::signal_set             signals_;
    Acceptor                            acceptor_;
    FuncOnAccept                        OnAccept;
    ConnectionCallBacks                 cb_;

    std::atomic<uint32>                 conn_id_;

    std::atomic<uint32>                 timer_id_;
    std::mutex                          timer_mutex_;
    std::map<uint32, TimerContext>      timer_map_;

    // logger timer
    boost::asio::deadline_timer         new_log_file_timer_;
    boost::asio::deadline_timer         server_logic_timer_;
};
extern std::shared_ptr<TcpServer> server;

#endif // _SERVER_H
