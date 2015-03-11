#ifndef _DRIVER_H
#define _DRIVER_H

#include "../ServerLib/TimerMgr.h"

#include "../RpcLib/rpc_service.h"

#include "../Common/Misc.h"

#include "GameScript.h"

#include "Protocol.pb.h"
#include "packet.pb.h"
#include "Gateway.pb.h"

#define SCRIPT_FRAME_INTERVAL 6 // ms

struct DriverStartupConfig
{
    std::string     service_name;
    std::string     rpc_server_addr;

    std::string     gateway_ip;
    std::string     gateway_port;
    int             pool_size;
};

class Driver
{
public:
    Driver();

public:
    void        Run(const DriverStartupConfig& config);
    void        Stop();

    void        Send(uint32 conn_id, const std::string& pkt);

private:
    void        InitRpcService(const std::string& name, const std::string& rpc_server);

private:
    // handler
    void        Dispatch(uint32 conn_id, const uint8 *buf, int32 len);

private:
    IoService                       ios_;
    GameScriptPtr                   script_;
    TimerMgrPtr                     timer_mgr_;
    uint32                          script_timer_;

    rpclib::RpcServiceProviderPtr   driver_;
};
extern Driver *driver;

#endif // _DRIVER_H
