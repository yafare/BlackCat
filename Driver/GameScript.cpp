#include "GameScript.h"

#include "../ServerLib/logger.hpp"

#include "LuaVM.h"

std::shared_ptr<GameScript> game_script;

GameScript::GameScript() : running_(false), vm_(0)
{
}

void GameScript::Init()
{
    vm_ = new LuaVM;
    if (vm_ == 0) {
        logger_->Log("Script env init fail");
        return;
    }
    vm_->Load("./z_scripts/game_main.lua");
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

ScriptFrameFunc GameScript::GetScriptFrameFunc()
{
    return std::bind(&GameScript::Run, shared_from_this());
}

void GameScript::Stop()
{
    running_ = false;
}
