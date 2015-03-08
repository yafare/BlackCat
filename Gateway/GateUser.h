#ifndef _GATEUSER_H
#define _GATEUSER_H

#include <map>
#include <memory>
#include <mutex>

#include "../ServerLib/ServerCommon.h"

#include "ClientGate.pb.h"

enum GateUserStatus
{
    GU_STATUS_INIT,                 // Î´µÇÂ½×´Ì¬
    GU_STATUS_IDLE,                 // µÇÂ½³É¹¦
    GU_STATUS_GAMING,               // ÓÎÏ·ÖÐ
};

class GateUser
{
public:
    GateUser(const ConnectionPtr& conn);

public:
    ConnectionPtr               GetConn();

public:
    GETTER_SETTER(std::string, uid);
    GETTER_SETTER(ClientGate::BasicUserInfo, user_info);

private:
    GateUserStatus              status_;
    ConnectionPtr               conn_;
};
using GateUserPtr = std::shared_ptr<GateUser>;

inline ConnectionPtr GateUser::GetConn()
{
    return conn_;
}

//////////////////////////////////////////////////////////////////////////
// gate user manager
class GateUserManager
{
public:
    void                AddUser(const ConnectionPtr& conn);
    void                RemoveUser(uint32 conn_id);

    GateUserPtr         GetUser(uint32 conn_id);
    std::string         GetUserId(uint32 conn_id);

private:
    std::map<uint32, GateUserPtr> game_users_;
    std::mutex mutex_;
};
inline GateUserManager& GetGateUserManager()
{
    static GateUserManager mgr;
    return mgr;
}

#endif // _GATEUSER_H
