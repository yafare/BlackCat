#include "Driver.h"

#include "../ServerLib/SessionMgr.h"

#include "GameScript.h"

Driver::Driver()
{
    logger_.reset(new services::logger(ios_, ""));

    client_.reset(new TcpClient(ios_));
    client_->SetCallBacks(GetCallBacks());

    script_.reset(new GameScript);
    script_->Init();

    timer_mgr_.reset(new TimerMgr(ios_));
    script_timer_ = timer_mgr_->AddTimer(script_->GetScriptFrameFunc(), SCRIPT_FRAME_INTERVAL);
}

void Driver::Run()
{
    ios_.run();
}

void Driver::Stop()
{
    timer_mgr_->Cancel(script_timer_);
    script_->Stop();
}

void Driver::Dispatch(uint32 conn_id, const uint8 *buf, uint32 len)
{
    script_->OnUserData(conn_id, buf, len);
}

void Driver::OnAccept(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Add(conn_id, conn);
    script_->OnUserConnected(conn_id);
}

void Driver::OnConnected(const ConnectionPtr& /*conn*/)
{
}

uint32 Driver::OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
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

void Driver::OnWrite(const ConnectionPtr& /*conn*/, uint32 /*len*/)
{
}

void Driver::OnDisconnect(const ConnectionPtr& conn)
{
    uint32 conn_id = conn->GetId();
    GetSessionMgr().Del(conn_id);
    script_->OnUserDisconnected(conn_id);
}
