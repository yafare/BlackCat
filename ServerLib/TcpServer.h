#ifndef _SERVER_H
#define _SERVER_H

#include <atomic>

#include "ServerCommon.h"
#include "io_service_pool.hpp"

class TcpServer : private boost::noncopyable
{
public:
    explicit TcpServer(const std::string& address, const std::string& port,
        std::size_t io_service_pool_size, const FuncOnAccept& func);

    void    SetCallBacks(const ConnectionCallBacks& cb);

    void    Run();

private:
    void    StartAccept();

private:
    io_service_pool                     io_service_pool_;
    boost::asio::signal_set             signals_;
    Acceptor                            acceptor_;
    FuncOnAccept                        OnAccept;
    ConnectionCallBacks                 cb_;

    std::atomic<uint32>                 conn_id_;
};
extern std::shared_ptr<TcpServer> server;

#endif // _SERVER_H
