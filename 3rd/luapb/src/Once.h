#ifndef _ONCE_H
#define _ONCE_H

#ifdef _MSC_VER

#include <mutex>

struct pthread_once_t
{
    pthread_once_t() : initialized_(false)
    {
    }
    std::mutex mutex_;
    bool initialized_;
};

inline void pthread_once(pthread_once_t *once, void(*init_func)())
{
    std::lock_guard<std::mutex> l(once->mutex_);
    if (!once->initialized_) {
        init_func();
        once->initialized_ = true;
    }
}

#else

#include <pthread.h>

#endif  // _MSC_VER

#endif  // _ONCE_H
