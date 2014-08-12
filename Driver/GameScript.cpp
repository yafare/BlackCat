#include "GameScript.h"

#include "../ServerLib/logger.hpp"
#include "../ServerLib/TcpServer.h"

#include "LuaVM.h"

std::shared_ptr<GameScript> game_script;

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
    if (!running_) {
        return;
    }
    Lock l(vm_mutex_);
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
