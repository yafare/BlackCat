#ifndef _IODISPATCHER_H
#define _IODISPATCHER_H

#include "../ServerLib/ServerCommon.h"

namespace PB
{
    class Packet;
}

class IoDispatcher
{
public:
    FuncOnAccept        GetAcceptCallBack();
    ConnectionCallBacks GetCallBacks();

private:
    // handler
    void                Dispatch(uint32 conn_id, const uint8 *buf, uint32 len);

    // call backs
    void                OnAccept(const ConnectionPtr& conn);

    void                OnConnected(const ConnectionPtr& conn);
    uint32              OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len);
    void                OnWrite(const ConnectionPtr& conn, uint32 len);
    void                OnDisconnect(const ConnectionPtr& conn);
};

inline FuncOnAccept IoDispatcher::GetAcceptCallBack()
{
    return std::bind(&IoDispatcher::OnAccept, this, std::placeholders::_1);
}

inline ConnectionCallBacks IoDispatcher::GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&IoDispatcher::OnConnected, this, std::placeholders::_1),
        std::bind(&IoDispatcher::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&IoDispatcher::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&IoDispatcher::OnDisconnect, this, std::placeholders::_1),
    };
    return cb;
}

#endif // _IODISPATCHER_H
