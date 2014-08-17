#include "TcpConnection.h"

#define CHECK_ERROR(e) \
    if (e) { \
        printf("%d - %s\n", e.value(), e.message().c_str()); \
        socket_.Shutdown(); \
        OnDisconnect(e); \
        return; \
    }

TcpConnection::TcpConnection(IoService& io_service) :
    socket_(io_service),
    io_service_(io_service)
{
}

void TcpConnection::Init(uint32 id)
{
    conn_id_ = id;
}

TcpConnection::~TcpConnection()
{
    printf("%s\n", __FUNCTION__);
}

void TcpConnection::Recv()
{
    socket_.Recv(std::bind(&TcpConnection::OnRead, shared_from_this(),
        std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::Send(const uint8 *buf, uint32 len)
{
    socket_.Send(std::bind(&TcpConnection::OnWrite, shared_from_this(),
        std::placeholders::_1, std::placeholders::_2), buf, len);
}

void TcpConnection::OnConnected(const ErrorCode& e)
{
    if (!e && cb_.OnConnected) {
        cb_.OnConnected(shared_from_this());
    }
}

void TcpConnection::OnRead(const ErrorCode& e, uint32 len)
{
    CHECK_ERROR(e);

    if (cb_.OnRead) {
        uint8 *buf = socket_.GetBuf();
        int cur_pos = recv_buf_.size();
        recv_buf_.resize(cur_pos + len);
        memcpy(&recv_buf_[0] + cur_pos, buf, len);
        uint32 proc_len = cb_.OnRead(shared_from_this(), &recv_buf_[0], recv_buf_.size());
        if (proc_len != 0) {
            recv_buf_.erase(recv_buf_.begin(), recv_buf_.begin() + proc_len);
        }
    }
    Recv();
}

void TcpConnection::OnWrite(const ErrorCode& e, uint32 len)
{
    CHECK_ERROR(e);

    if (cb_.OnWrite) {
        cb_.OnWrite(shared_from_this(), len);
    }
}

void TcpConnection::OnDisconnect(const ErrorCode& /*e*/)
{
    printf("disconnect: %d\n", GetId());
    if (cb_.OnDisconnect) {
        cb_.OnDisconnect(shared_from_this());
    }
}
