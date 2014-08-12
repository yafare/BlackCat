#include "SessionMgr.h"

void SessionMgr::Add(uint32 id, const ConnectionPtr& conn)
{
    Lock l(mutex_);
    connections_[id] = conn;
}

void SessionMgr::Del(uint32 id)
{
    Lock l(mutex_);
    connections_.erase(id);
}

ConnectionPtr SessionMgr::Get(uint32 id)
{
    Lock l(mutex_);
    auto it = connections_.find(id);
    if (it == connections_.end()) {
        return ConnectionPtr();
    }
    return it->second;
}
