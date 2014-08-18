#ifndef _DRIVER_H
#define _DRIVER_H

#include "../ServerLib/TcpClient.h"
#include "../ServerLib/TimerMgr.h"

#include "GameScript.h"

#define SCRIPT_FRAME_INTERVAL 6 // ms

class Driver
{
public:
    Driver();

public:
    void                Run(const std::string& ip, const std::string& port);
    void                Stop();

    void                Send(uint32 conn_id, const std::string& pkt);

private:
    void                LoginToGate();

private:
    ConnectionCallBacks GetCallBacks();

private:
    // handler
    void                Dispatch(uint32 conn_id, const uint8 *buf, uint32 len);

    // call backs
    void                OnAccept(const ConnectionPtr& conn);

    void                OnConnected(const ConnectionPtr& conn, bool success);
    uint32              OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len);
    void                OnWrite(const ConnectionPtr& conn, uint32 len);
    void                OnDisconnect(const ConnectionPtr& conn);

private:
    IoService           ios_;
    TcpClientPtr        client_;
    GameScriptPtr       script_;
    TimerMgrPtr         timer_mgr_;

    std::string         gate_ip_, gate_port_;

    uint32              script_timer_;
};
extern Driver *driver;

inline ConnectionCallBacks Driver::GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&Driver::OnConnected, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Driver::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&Driver::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Driver::OnDisconnect, this, std::placeholders::_1),
    };
    return cb;
}

#endif // _DRIVER_H
