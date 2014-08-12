#include "GameScript.h"

#include "../ServerLib/logger.hpp"
#include "../ServerLib/TcpServer.h"
#include "../ServerLib/SessionMgr.h"
#include "../ServerLib/TcpConnection.h"

#include "LuaVM.h"

std::shared_ptr<GameScript> game_script;

#define CHECK_AND_LOCK() \
    if (!running_ || vm_ == 0) { \
        return; \
    } \
    Lock l(vm_mutex_);

GameScript::GameScript() : running_(false), vm_(0)
{
}

void GameScript::Init()
{
    script_timer_ = server->SetTimer(game_script->GetScriptFrameFunc(), SCRIPT_FRAME_INTERVAL);

    vm_ = new LuaVM;
    if (vm_ == 0) {
        logger_->Log("Script env init fail");
        return;
    }
    if (!vm_->Load("./game_main.lua")) {
        logger_->Log("Load game main script fail");
        return;
    }

    running_ = true;
}

void GameScript::Run()
{
    CHECK_AND_LOCK();

    vm_->Call("game_main");
}

TimerFunc GameScript::GetScriptFrameFunc()
{
    return std::bind(&GameScript::Run, shared_from_this());
}

void GameScript::Stop()
{
    running_ = false;
    server->KillTimer(script_timer_);
}

void GameScript::OnUserConnected(uint32 conn_id)
{
    CHECK_AND_LOCK();

    vm_->Call("OnUserConnected", conn_id);
}

void GameScript::OnUserDisconnected(uint32 conn_id)
{
    CHECK_AND_LOCK();

    vm_->Call("OnUserDisconnected", conn_id);
}

void GameScript::OnUserData(uint32 conn_id, const uint8 *buf, uint32 len)
{
    CHECK_AND_LOCK();

    lua_State *L = vm_->GetLuaState();
    lua_getglobal(L, "OnUserData");
    if (!lua_isfunction(L, 1)) {
        return;
    }
    lua_pushinteger(L, conn_id);
    lua_pushlstring(L, (const char *)buf, len);
    lua_pcall(L, 2, 0, 0);
}

void GameScript::SendData(uint32 conn_id, const uint8 *buf, uint32 len)
{
    CHECK_AND_LOCK();

    auto conn = GetSessionMgr().Get(conn_id);
    if (conn == 0) {
        return;
    }
    conn->Send(buf, len);
}
