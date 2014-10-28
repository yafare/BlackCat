#ifndef _SERVER_H
#define _SERVER_H

#include <atomic>
#include <unordered_set>

#include "ServerCommon.h"
#include "io_service_pool.hpp"
#include "IndexMgr.h"

class TcpServer : private boost::noncopyable
{
public:
    explicit TcpServer(const std::string& address, const std::string& port,
        std::size_t io_service_pool_size, const FuncOnAccept& func);

    void    SetCallBacks(const ConnectionCallBacks& cb);

    void    Run();

    boost::asio::io_service& IoService() { return io_service_pool_.get_io_service(); }

private:
    void    StartAccept();
    void    ConnectionDown(uint32 id);

private:
    io_service_pool                     io_service_pool_;
    boost::asio::signal_set             signals_;
    Acceptor                            acceptor_;
    FuncOnAccept                        OnAccept;
    ConnectionCallBacks                 cb_;

    IndexMgr<uint32>                    conn_id_mgr_;
};
extern std::shared_ptr<TcpServer> server;

inline void TcpServer::SetCallBacks(const ConnectionCallBacks& cb)
{
    cb_ = cb;
}

inline void TcpServer::Run()
{
    io_service_pool_.run();
}

inline void TcpServer::ConnectionDown(uint32 id)
{
    conn_id_mgr_.PutId(id);
}

#endif // _SERVER_H
