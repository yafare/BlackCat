#ifndef _GAME_SCRIPT_H
#define _GAME_SCRIPT_H

#include <mutex>
#include <memory>
#include <functional>

#include "../ServerLib/ServerCommon.h"

class LuaVM;
class GameScript : public std::enable_shared_from_this<GameScript>
{
public:
    GameScript();

public:
    void            Init();
    void            Stop();

    ScriptFrameFunc GetScriptFrameFunc();

private:
    // called by Driver with an interval 5ms
    void            Run();

private:
    LuaVM          *vm_;
    std::mutex      vm_mutex_;
    bool            running_;
};
using Lock = std::lock_guard<std::mutex>;
extern std::shared_ptr<GameScript> game_script;

#endif  // _GAME_SCRIPT_H
