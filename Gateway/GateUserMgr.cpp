#include "GateUser.h"

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/TcpConnection.h"

void GateUserManager::AddUser(const ConnectionPtr& conn)
{
    GateUserPtr gu(new GateUser(conn));

    Lock l(mutex_);
    unknown_users_[conn->GetId()] = gu;
}

bool GateUserManager::ChangeUserType(uint32 conn_id, GateUserType type)
{
    Lock l(mutex_);
    auto it = unknown_users_.find(conn_id);
    if (it == unknown_users_.end()) {
        return false;
    }

    const GateUserPtr& gu = it->second;
    if (type == GU_TYPE_GAME_SERVER) {
        server_users_[conn_id] = gu;
    } else {
        game_users_[conn_id] = gu;
    }
    unknown_users_.erase(it);
    return true;
}

void GateUserManager::RemoveUser(uint32 conn_id)
{
    Lock l(mutex_);
    {
        auto it = server_users_.find(conn_id);
        if (it != server_users_.end()) {
            server_users_.erase(it);
            return;
        }
    }
    {
        auto it = unknown_users_.find(conn_id);
        if (it != unknown_users_.end()) {
            unknown_users_.erase(it);
            return;
        }
    }
    {
        auto it = game_users_.find(conn_id);
        if (it != game_users_.end()) {
            game_users_.erase(it);
            return;
        }
    }
}

GateUserPtr GateUserManager::GetUser(uint32 conn_id)
{
    Lock l(mutex_);
    {
        auto it = server_users_.find(conn_id);
        if (it != server_users_.end()) {
            return it->second;
        }
    }
    {
        auto it = unknown_users_.find(conn_id);
        if (it != unknown_users_.end()) {
            return it->second;
        }
    }
    {
        auto it = game_users_.find(conn_id);
        if (it != game_users_.end()) {
            return it->second;
        }
    }
    return 0;
}

std::string GateUserManager::GetUserId(uint32 conn_id)
{
    GateUserPtr user = GetUser(conn_id);
    if (user == 0) {
        return std::string("");
    }
    return user->Getuid();
}
