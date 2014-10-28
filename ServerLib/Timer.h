#ifndef _TIMER_H
#define _TIMER_H

#include "../ServerLib/ServerCommon.h"

#include <boost/timer/timer.hpp>

struct Timer
{
    int32 GetRestTick()
    {
        boost::timer::cpu_times elapsed_times(timer.elapsed());
        boost::timer::nanosecond_type elapsed(elapsed_times.wall);
        return (int32)((interval * 1000000LL - elapsed) / 1000000LL);
    }
    uint32 interval;
    TimerFunc func;
    boost::timer::cpu_timer timer;
};

#endif // _TIMER_H
