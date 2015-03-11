#include "Driver.h"

#include "GameScript.h"

#include "Protocol.pb.h"
#include "Gateway.pb.h"

using namespace Protocol;
using namespace GatewayServer;

void Driver::Send(uint32 conn_id, const std::string& pkt)
{
    printf("send pkt to %d, size: %d\n", conn_id, pkt.size());
}

void Driver::Dispatch(uint32 conn_id, const uint8 *buf, int32 len)
{
    script_->OnUserData(conn_id, buf, len);
}
