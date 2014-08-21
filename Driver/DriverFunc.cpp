#include "Driver.h"

#include "../ServerLib/md5.h"

#include "Protocol.pb.h"
#include "Gateway.pb.h"

using namespace Protocol;
using namespace GatewayServer;

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
    LoginRequest request;

    EnumServerType type = ST_GameServer;
    request.set_server_type(type);

    char *account = "just_for_test";
    request.set_account(account);

    std::string tmp(account + std::to_string(type));
    std::string key = GetMD5((md5byte *)tmp.c_str(), tmp.size());
    request.set_secure_key(key);

    SendToGate(SERVER_LOGIN_REQUEST, request);
}
