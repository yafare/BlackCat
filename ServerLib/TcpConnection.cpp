#include "TcpConnection.h"

#define CHECK_ERROR(e) \
    if (e) { \
        socket_.Shutdown(); \
        OnDisconnect(e); \
        return; \
    }

TcpConnection::TcpConnection(IoService& io_service, DisconnectCallBack disconnect_func) :
    conn_id_(INVALID_CONN_ID),
    socket_(io_service),
    io_service_(io_service),
    disconnect_func_(disconnect_func),
    writting_msg_(false)
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

void TcpConnection::Send(const uint8 *buf, int len)
{
    BufferPtr buffer(new std::vector<uint8>(buf, buf + len));
    io_service_.post(std::bind(&TcpConnection::RealSend, shared_from_this(), buffer));
}
void TcpConnection::RealSend(const BufferPtr& buffer)
{
    send_buf_.push_back(buffer);
    if (writting_msg_) {
        return;
    }

    writting_msg_ = true;
    socket_.Send(std::bind(&TcpConnection::OnWrite, shared_from_this(),
        std::placeholders::_1, std::placeholders::_2), &(*buffer->begin()), buffer->size());
}

void TcpConnection::OnConnected(const ErrorCode& e)
{
    if (cb_.OnConnected) {
        cb_.OnConnected(shared_from_this(), !e);
    }
}

void TcpConnection::OnRead(const ErrorCode& e, int32 len)
{
    CHECK_ERROR(e);

    if (cb_.OnRead) {
        uint8 *buf = socket_.GetBuf();
        int32 cur_pos = recv_buf_.size();
        recv_buf_.resize(cur_pos + len);
        memcpy(&recv_buf_[0] + cur_pos, buf, len);
        int32 proc_len = cb_.OnRead(shared_from_this(), &recv_buf_[0], recv_buf_.size());
        if (proc_len != 0) {
            recv_buf_.erase(recv_buf_.begin(), recv_buf_.begin() + proc_len);
        }
    }
    Recv();
}

void TcpConnection::OnWrite(const ErrorCode& e, int32 len)
{
    writting_msg_ = false;

    CHECK_ERROR(e);

    if (cb_.OnWrite) {
        cb_.OnWrite(shared_from_this(), len);
    }

    send_buf_.pop_front();

    // continue send
    if (!send_buf_.empty()) {
        auto buffer = send_buf_.front();

        writting_msg_ = true;
        socket_.Send(std::bind(&TcpConnection::OnWrite, shared_from_this(),
            std::placeholders::_1, std::placeholders::_2), &(*buffer->begin()), static_cast<int32>(buffer->size()));
    }
}

void TcpConnection::OnDisconnect(const ErrorCode& /*e*/)
{
    printf("disconnect: %d\n", GetId());
    if (cb_.OnDisconnect) {
        cb_.OnDisconnect(shared_from_this());
    }
    if (disconnect_func_) {
        disconnect_func_(GetId());
    }
}
