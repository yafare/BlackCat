#include "Gateway.h"

#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"
#include "../ServerLib/SessionMgr.h"

Gateway::Gateway()
{
}

void Gateway::Run(const std::string& ip, const std::string& port, int pool_size)
{
    server.reset(new TcpServer(ip, port, pool_size, GetAcceptCallBack()));
    server->SetCallBacks(GetCallBacks());

    server->Run();
}

void Gateway::Dispatch(uint32 conn_id, const uint8 *buf, uint32 len)
{
}

void Gateway::OnAccept(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Add(conn_id, conn);
}

void Gateway::OnConnected(const ConnectionPtr& /*conn*/, bool /*success*/)
{
}

uint32 Gateway::OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
{
    if (len > MAX_RECV_BUF) {
        return len;
    }

    uint32 conn_id = conn->GetId();

    uint32 total_len = 0;
    while (len > sizeof(PacketHeader)) {
        PacketHeader *p = (PacketHeader *)buf;
        uint32 cur_len = p->len + sizeof(PacketHeader);
        if (len < p->len) {
            break;
        }

        Dispatch(conn_id, p->buf, p->len);

        total_len += cur_len;
        buf += cur_len;
        len -= cur_len;
    }

    return total_len;
}

void Gateway::OnWrite(const ConnectionPtr& /*conn*/, uint32 /*len*/)
{
}

void Gateway::OnDisconnect(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Del(conn_id);
}
