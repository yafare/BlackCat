#include "Driver.h"

void Driver::Send(uint32 conn_id, const std::string& pkt)
{
    printf("send pkt to %d, size: %d\n", conn_id, pkt.size());
}

void Driver::Dispatch(uint32 conn_id, const uint8 *buf, uint32 len)
{
    script_->OnUserData(conn_id, buf, len);
}

void Driver::LoginToGate()
{
}
