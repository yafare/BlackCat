#include "IoDispatcher.h"

#include "../ServerLib/SessionMgr.h"
#include "../ServerLib/TcpConnection.h"

#include "GameScript.h"

void IoDispatcher::Dispatch(uint32 conn_id, const uint8 *buf, uint32 len)
{
    game_script->OnUserData(conn_id, buf, len);
}

void IoDispatcher::OnAccept(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Add(conn_id, conn);
    game_script->OnUserConnected(conn_id);
}

void IoDispatcher::OnConnected(const ConnectionPtr& /*conn*/)
{
}

uint32 IoDispatcher::OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
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

void IoDispatcher::OnWrite(const ConnectionPtr& /*conn*/, uint32 /*len*/)
{
}

void IoDispatcher::OnDisconnect(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Del(conn_id);
    game_script->OnUserDisconnected(conn_id);
}
