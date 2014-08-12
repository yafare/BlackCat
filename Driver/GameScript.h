#ifndef _GAME_SCRIPT_H
#define _GAME_SCRIPT_H

#include <mutex>
#include <memory>
#include <functional>

#include "../ServerLib/ServerCommon.h"

#define SCRIPT_FRAME_INTERVAL 6 // ms

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
    std::mutex      vm_mutex_;
    bool            running_;
    uint32          script_timer_;
};
using Lock = std::lock_guard<std::mutex>;
extern std::shared_ptr<GameScript> game_script;

#endif  // _GAME_SCRIPT_H
