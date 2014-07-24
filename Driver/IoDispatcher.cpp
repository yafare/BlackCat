#include "IoDispatcher.h"

#include "../ServerLib/SessionMgr.h"
#include "../ServerLib/TcpConnection.h"

void IoDispatcher::Dispatch(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
{
    conn->Send(buf, len);
}

void IoDispatcher::OnConnected(ConnectionPtr conn)
{
}

uint32 IoDispatcher::OnRead(ConnectionPtr conn, const uint8 *buf, uint32 len)
{
    if (len > MAX_RECV_BUF) {
        return len;
    }

    uint32 total_len = 0;
    while (len > sizeof(PacketHeader)) {
        PacketHeader *p = (PacketHeader *)buf;
        uint32 cur_len = p->len + sizeof(PacketHeader);
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

void IoDispatcher::OnWrite(ConnectionPtr /*conn*/, uint32 /*len*/)
{
}

void IoDispatcher::OnDisconnect(ConnectionPtr conn)
{
    GetSessionMgr().Del(conn);
}
