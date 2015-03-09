#include "GenericServer.h"

#include "TcpServer.h"
#include "TcpConnection.h"

GenericServer::GenericServer()
{
}

void GenericServer::Run(const std::string& ip, const std::string& port, int pool_size)
{
    TcpServerStartupConfig config = {
        ip, port, pool_size, GetAcceptCallBack(this), GetCallBacks(this)
    };
    server.reset(new TcpServer(config));
    server->Run();
}

void GenericServer::OnAccept(const ConnectionPtr& conn)
{
}

void GenericServer::OnConnected(const ConnectionPtr& /*conn*/, bool /*success*/)
{
}

int32 GenericServer::OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len)
{
    if (len >= MAX_RECV_BUF) {
        conn->Shutdown();
        return len;
    }

    int32 total_len = 0;
    while (len > sizeof(PacketHeader)) {
        PacketHeader *p = (PacketHeader *)buf;
        int32 cur_len = p->len + sizeof(PacketHeader);
        if (len < p->len) {
            break;
        }

        Dispatch(conn, p->buf, p->len);

        total_len += cur_len;
        buf += cur_len;
        len -= cur_len;
    }

    return total_len;
}

void GenericServer::OnWrite(const ConnectionPtr& /*conn*/, int32 /*len*/)
{
}

void GenericServer::OnDisconnect(const ConnectionPtr& conn)
{
}
