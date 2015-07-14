#ifndef _SERVERCOMMON_H
#define _SERVERCOMMON_H

#include <mutex>
#include <functional>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "../Common/Misc.h"
#include "../Common/Types.h"

#include "Codec.h"
#include "logger.hpp"

#define MAX_RECV_BUF (32*1024)
#pragma pack(push, 1)
#ifdef _MSC_VER
#pragma warning(disable:4200)
#endif // _MSC_VER
struct PacketHeader
{
    int32 len;
    uint8 buf[0];
};
#pragma pack(pop)

using Lock                  = std::lock_guard<std::mutex>;
using ErrorCode             = boost::system::error_code;
using IoService             = boost::asio::io_service;
using EndPoint              = boost::asio::ip::tcp::endpoint;
using Resolver              = boost::asio::ip::tcp::resolver;
using Query                 = boost::asio::ip::tcp::resolver::query;
using TcpSocket             = boost::asio::ip::tcp::socket;
using Acceptor              = boost::asio::ip::tcp::acceptor;
using TimerFunc             = std::function<void()>;

class TcpConnection;
using ConnectionPtr         = std::shared_ptr<TcpConnection>;

using FuncOnAccept          = std::function<void(const ConnectionPtr&)>;
using FuncOnConnect         = std::function<void(const ConnectionPtr&, bool)>;
using FuncOnRead            = std::function<int32(const ConnectionPtr&, const uint8 *, int32)>;
using FuncOnWrite           = std::function<void(const ConnectionPtr&, int32)>;
using FuncOnDisconnect      = std::function<void(const ConnectionPtr&)>;

struct ConnectionCallBacks
{
    FuncOnConnect       OnConnected;
    FuncOnRead          OnRead;
    FuncOnWrite         OnWrite;
    FuncOnDisconnect    OnDisconnect;
};

struct AutoLogFunc
{
    AutoLogFunc(const char *func) : func_(func)
    {
        LOG("enter %s", func);
    }
    ~AutoLogFunc()
    {
        LOG("leave %s", func_);
    }
    const char *func_;
};
#define DEF_AUTO_LOG_FUNC() \
    AutoLogFunc auto_log_func(__FUNCTION__);

#endif // _SERVERCOMMON_H
