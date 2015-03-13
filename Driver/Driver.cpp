#include "Driver.h"

#include "GameScript.h"
#include "LuaBinder.h"
#include "LuaVM.h"

Driver *driver = 0;

Driver::Driver()
{
    script_.reset(new GameScript);
    script_->Init();

    timer_mgr_.reset(new TimerMgr(ios_));
    script_timer_ = timer_mgr_->AddTimer(script_->GetScriptFrameFunc(), SCRIPT_FRAME_INTERVAL);
}

void Driver::Run(const DriverStartupConfig& config)
{
    gateway_name_ = config.gateway_name;

    InitRpcService(config.service_name, config.rpc_server_addr);

    ios_.run();
}

void Driver::InitRpcService(const std::string& name, const std::string& rpc_server)
{
    driver_.reset(new rpclib::RpcServiceProvider(name));
    driver_->Connect(rpc_server);
}

void Driver::Stop()
{
    timer_mgr_->Cancel(script_timer_);
    script_->Stop();
}
