#include "GameScript.h"

#include "../ServerLib/logger.hpp"
#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"

#include "LuaVM.h"
#include "LuaBinder.h"

#define CHECK_VM_RUNNING() \
    if (!running_ || vm_ == 0) { \
        return; \
    }

GameScript::GameScript() : running_(false), vm_(0)
{
}

void GameScript::Init()
{
    vm_ = new LuaVM;
    if (vm_ == 0) {
        LOG("Script env init fail");
        return;
    }

    LuaBinder binder;
    binder.Bind(vm_);

    if (!vm_->Load("./game_main.lua")) {
        LOG("Load game main script fail");
        return;
    }

    running_ = true;
}

void GameScript::Run()
{
    CHECK_VM_RUNNING();

    vm_->Call("game_main");
}

TimerFunc GameScript::GetScriptFrameFunc()
{
    return std::bind(&GameScript::Run, shared_from_this());
}

void GameScript::Stop()
{
    running_ = false;
}

void GameScript::OnUserConnected(uint32 conn_id)
{
    CHECK_VM_RUNNING();

    vm_->Call("OnUserConnected", conn_id);
}

void GameScript::OnUserDisconnected(uint32 conn_id)
{
    CHECK_VM_RUNNING();

    vm_->Call("OnUserDisconnected", conn_id);
}

void GameScript::OnUserData(uint32 conn_id, const uint8 *buf, uint32 len)
{
    CHECK_VM_RUNNING();

    std::string pkt;
    pkt.append((const char *)buf, len);
    vm_->Call("OnUserData", conn_id, pkt);
}
