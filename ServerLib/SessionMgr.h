#ifndef _SESSIONMGR_H
#define _SESSIONMGR_H

#include <map>
#include <atomic>
#include <mutex>

#include "ServerCommon.h"

class SessionMgr
{
public:
    void    Add(uint32 id, const ConnectionPtr& conn);
    void    Del(uint32 id);

    ConnectionPtr Get(uint32 id);

private:
    std::map<int32, ConnectionPtr> connections_;
    std::mutex mutex_;
};

inline SessionMgr& GetSessionMgr()
{
    static SessionMgr mgr;
    return mgr;
}

#endif // _SESSIONMGR_H
