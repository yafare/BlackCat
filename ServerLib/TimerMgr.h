#ifndef _TIMERMGR_H
#define _TIMERMGR_H

#include "Timer.h"

#include <unordered_map>
#include <atomic>

#define LOGIC_TIMER_INTERVAL  2 // ms

// TODO: modify unordered_map to time wheel
class TimerMgr
{
public:
    TimerMgr(IoService& ios);

public:
    // create a timer, interval: ms, return the ID of timer
    uint32      AddTimer(const TimerFunc& func, uint32 interval);
    void        Cancel(uint32 timer_id);

    int32       GetRestTime(uint32 time_id);

    void        ResetLogicTimer();

private:
    IoService&                          ios_;
    std::atomic<uint32>                 timer_id_;
    std::unordered_map<uint32, Timer>   timer_map_;
    boost::asio::deadline_timer         server_logic_timer_;
};
using TimerMgrPtr = std::shared_ptr<TimerMgr>;

#endif // _TIMERMGR_H
