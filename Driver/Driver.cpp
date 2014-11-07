#include "Driver.h"

#include "GameScript.h"
#include "LuaBinder.h"
#include "LuaVM.h"

Driver *driver = 0;

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

void Driver::Run(const std::string& ip, const std::string& port)
{
    gate_ip_ = ip, gate_port_ = port;
    client_->Connect(gate_ip_, gate_port_);
    ios_.run();
}

void Driver::Stop()
{
    timer_mgr_->Cancel(script_timer_);
    script_->Stop();
}

void Driver::OnConnected(const ConnectionPtr& /*conn*/, bool success)
{
    if (!success) {
        LOG("Gate server connect fail, retry now");
        client_->Connect(gate_ip_, gate_port_);
    } else {
        printf("Successfully connect to gate server\n");
        LoginToGate();
    }
}

uint32 Driver::OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len)
{
    if (len > MAX_RECV_BUF) {
        return len;
    }

    uint32 conn_id = conn->GetId();

    int32 total_len = 0;
    while (len > sizeof(PacketHeader)) {
        PacketHeader *p = (PacketHeader *)buf;
        int32 cur_len = p->len + sizeof(PacketHeader);
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

void Driver::OnWrite(const ConnectionPtr& /*conn*/, int32 /*len*/)
{
}

void Driver::OnDisconnect(const ConnectionPtr& conn)
{
    LOG("disconnected from gate server, try to reconnect");
    client_->Shutdown();
    client_->Connect(gate_ip_, gate_port_);
}
