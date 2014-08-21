#include "Gateway.h"

#include "../ServerLib/md5.h"
#include "../ServerLib/fasthash.h"
#include "../ServerLib/TcpConnection.h"

#include "Protocol.pb.h"
#include "packet.pb.h"
#include "Gateway.pb.h"
#include "ClientGate.pb.h"

#include "GateUser.h"

using namespace Protocol;
using namespace GatewayServer;

void Gateway::HandlePacket(const ConnectionPtr& conn, const PB::Packet& packet)
{
    switch (packet.command())
    {
    case SERVER_LOGIN_REQUEST:
        HandleServerLogin(conn, packet);
        break;
    case CLIENT_LOGIN_REQUEST:
        HandleClientLogin(conn, packet);
        break;
    }
}

void Gateway::HandleServerLogin(const ConnectionPtr& conn, const PB::Packet& packet)
{
    GatewayServer::LoginRequest request;

    if (!request.ParseFromString(packet.serialized())) {
        LOG("invalid server login packet, shutdown this connection");
        conn->Shutdown();
        return;
    }

    EnumServerType type = request.server_type();
    std::string account = request.account() + std::to_string(type);
    std::string key = request.secure_key();

    std::string result = GetMD5((uint8 *)account.c_str(), account.size());
    if (result != key) {
        LOG("invalid server login packet, wrong key, shutdown this connection");
        conn->Shutdown();
        return;
    }

    GetGateUserManager().ChangeUserType(conn->GetId(),
        type == ST_GameServer ? GU_TYPE_GAME_SERVER : GU_TYPE_RECORD_SERVER);
}

void Gateway::HandleClientLogin(const ConnectionPtr& conn, const PB::Packet& packet)
{
    ClientGate::LoginRequest request;

    if (!request.ParseFromString(packet.serialized())) {
        LOG("invalid client login packet, shutdown this connection");
        conn->Shutdown();
        return;
    }

    std::string mac = request.mac();
    std::string device_id = request.device_id();

    std::string hash = mac + "P36J9FH3HF0fujweu9we9dcjn3488CRY0X47CH" + device_id;
    std::ostringstream ohash;
    ohash << fasthash64(hash.c_str(), hash.length(), 0);
    if (request.secure_key() != ohash.str()) {
        LOG("invalid client login packet, wrong secure key, shutdown this connection");
        conn->Shutdown();
        return;
    }

    std::string acc = request.account();
    std::string nick = request.nick();
    printf("[%s/%s] request to login\n", acc.c_str(), nick.c_str());

    GetGateUserManager().ChangeUserType(conn->GetId(), GU_TYPE_PLAYER);
}
