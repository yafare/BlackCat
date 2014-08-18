#ifndef _TIMER_H
#define _TIMER_H

#include "../ServerLib/ServerCommon.h"

#include <boost/timer/timer.hpp>

struct Timer
{
    uint32 interval;
    TimerFunc func;
    boost::timer::cpu_timer timer;
};

#endif // _TIMER_H
