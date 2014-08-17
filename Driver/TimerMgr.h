#ifndef _TIMERMGR_H
#define _TIMERMGR_H

#include "Timer.h"

#include <map>
#include <atomic>

#define LOGIC_TIMER_INTERVAL  2 // ms

class TimerMgr
{
public:
    TimerMgr(IoService& ios);

public:
    // create a timer, interval: ms, return the ID of timer
    uint32      AddTimer(const TimerFunc& func, uint32 interval);
    void        Cancel(uint32 timer_id);

    void        ResetLogicTimer();

private:
    IoService&                          ios_;
    std::atomic<uint32>                 timer_id_;
    std::map<uint32, Timer>             timer_map_;
    boost::asio::deadline_timer         server_logic_timer_;
};
using TimerMgrPtr = std::shared_ptr<TimerMgr>;

#endif // _TIMERMGR_H
