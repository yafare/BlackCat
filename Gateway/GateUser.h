#ifndef _GATEUSER_H
#define _GATEUSER_H

#include <map>
#include <memory>
#include <mutex>

#include "../ServerLib/ServerCommon.h"

#include "ClientGate.pb.h"

enum GateUserType
{
    GU_TYPE_NA,                     // 初始化未知状态
    GU_TYPE_GAME_SERVER,            // 游戏服务器
    GU_TYPE_RECORD_SERVER,          // 数据服务器
    GU_TYPE_PLAYER,                 // 普通玩家
};

enum GateUserStatus
{
    GU_STATUS_INIT,                 // 未登陆状态
    GU_STATUS_IDLE,                 // 登陆成功
    GU_STATUS_GAMING,               // 游戏中
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
    GateUserType                type_;
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
    bool                ChangeUserType(uint32 conn_id, GateUserType type);
    void                RemoveUser(uint32 conn_id);

    GateUserPtr         GetUser(uint32 conn_id);
    std::string         GetUserId(uint32 conn_id);

private:
    std::map<uint32, GateUserPtr> unknown_users_, game_users_, server_users_;
    std::mutex mutex_;
};
inline GateUserManager& GetGateUserManager()
{
    static GateUserManager mgr;
    return mgr;
}

#endif // _GATEUSER_H
