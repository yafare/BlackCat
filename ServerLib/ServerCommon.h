#ifndef _SERVERCOMMON_H
#define _SERVERCOMMON_H

#include <mutex>
#include <functional>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>

#include "../Common/Types.h"
#include "logger.hpp"

#define MAX_RECV_BUF (8*1024)
#pragma pack(push, 1)
#ifdef _MSC_VER
#pragma warning(disable:4200)
#endif // _MSC_VER
struct PacketHeader
{
    uint32 len;
    uint8 buf[0];
};
#pragma pack(pop)

using Lock      = std::lock_guard<std::mutex>;
using ErrorCode = boost::system::error_code;
using IoService = boost::asio::io_service;
using TcpSocket = boost::asio::ip::tcp::socket;
using Acceptor  = boost::asio::ip::tcp::acceptor;

class TcpConnection;
using ConnectionPtr = std::shared_ptr<TcpConnection>;

using FuncOnAccept          = std::function<void(ConnectionPtr)>;
using FuncOnConnect         = std::function<void(ConnectionPtr)>;
using FuncOnRead            = std::function<uint32(ConnectionPtr, const uint8 *, uint32)>;
using FuncOnWrite           = std::function<void(ConnectionPtr, uint32)>;
using FuncOnDisconnect      = std::function<void(ConnectionPtr)>;
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
        logger_->Log("enter %s", func);
    }
    ~AutoLogFunc()
    {
        logger_->Log("leave %s", func_);
    }
    const char *func_;
};
#define DEF_AUTO_LOG_FUNC() \
    AutoLogFunc auto_log_func(__FUNCTION__);

#endif // _SERVERCOMMON_H
