#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/GenericServer.h"

class DataServer : public GenericServer
{
public:
    DataServer();

private:
    // handler
    virtual void        Dispatch(const ConnectionPtr& conn, const uint8 *buf, int32 len);
};

#endif // _GATEWAY_H
