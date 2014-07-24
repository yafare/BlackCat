#include "SessionMgr.h"

#include "TcpConnection.h"

void SessionMgr::Add(ConnectionPtr conn)
{
    Lock l(mutex_);
    connections_[conn->GetId()] = conn;
}

void SessionMgr::Del(ConnectionPtr conn)
{
    Lock l(mutex_);
    connections_.erase(conn->GetId());
}
