#include <stdio.h>

#include "../ServerLib/TcpClient.h"

#include "packet.pb.h"
#include "Protocol.pb.h"
#include "ClientGate.pb.h"

using namespace Protocol;
using namespace ClientGate;

IoService ios;
TcpClientPtr client;

const char *device_name = "MI 2S";

void SendPacket(const PB::Packet& pkt)
{
    uint32 size = pkt.ByteSize();
    uint8 *buf = (uint8 *)alloca(size + 4);
    memcpy(buf, &size, 4);
    pkt.SerializeToArray(buf + 4, size);
    client->Send(buf, size + 4);
}
template <typename T>
void SendPacket(uint32 cmd, const T& pkt)
{
    PB::Packet packet;
    packet.set_version(PROTOCOL_VERSION);
    packet.set_command(CLIENT_LOGIN_REQUEST);
    pkt.SerializeToString(packet.mutable_serialized());
    SendPacket(packet);
}

void ConnectToGate()
{
    client->Connect("127.0.0.1", "8990");
}

void Login()
{
    std::string mac_("test");
    std::string user = mac_ + "@test.me";
    std::string deviceid = "";

    LoginRequest request;
    request.set_login_type(enumLoginTypeRegisterNewUser);
    request.set_account(user);
    request.set_password(mac_);
    request.set_nick(device_name);
    request.set_gender(enumGenderMale);
    request.set_device_type(enumDeviceTypeAndroid);
    request.set_device_id(deviceid);
    request.set_mac(mac_);
    request.set_channel("unknown");
    request.set_secure_key("unknown");
    request.set_version(1000);

    SendPacket(CLIENT_LOGIN_REQUEST, request);
}

void OnConnected(const ConnectionPtr& conn, bool success)
{
    printf("connect %s\n", success ? "success" : "fail");
    if (success) {
        Login();
    } else {
        ConnectToGate();
    }
}
uint32 OnRead(const ConnectionPtr& conn, const uint8 *buf, uint32 len)
{
    return len;
}
void OnWrite(const ConnectionPtr& conn, uint32 len)
{
}
void OnDisconnect(const ConnectionPtr& conn)
{
    ConnectToGate();
}

inline ConnectionCallBacks GetCallBacks()
{
    ConnectionCallBacks cb = {
        std::bind(&OnConnected, std::placeholders::_1, std::placeholders::_2),
        std::bind(&OnRead, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&OnWrite, std::placeholders::_1, std::placeholders::_2),
        std::bind(&OnDisconnect, std::placeholders::_1),
    };
    return cb;
}

int main()
{
    client.reset(new TcpClient(ios));
    client->SetCallBacks(GetCallBacks());

    ConnectToGate();

    ios.run();
}
