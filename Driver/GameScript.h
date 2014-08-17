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

    // script callbacks
    void            OnUserConnected(uint32 conn_id);
    void            OnUserDisconnected(uint32 conn_id);
    void            OnUserData(uint32 conn_id, const uint8 *buf, uint32 len);

    // function to lua
    void            SendData(uint32 conn_id, const uint8 *buf, uint32 len);

    TimerFunc       GetScriptFrameFunc();

private:
    // called by Driver with an interval 5ms
    void            Run();

private:
    LuaVM          *vm_;
    bool            running_;
};
using GameScriptPtr = std::shared_ptr<GameScript>;

#endif  // _GAME_SCRIPT_H
