#include "GateUser.h"

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/TcpConnection.h"

void GateUserManager::AddUser(const ConnectionPtr& conn)
{
    GateUserPtr gu(new GateUser(conn));

    Lock l(mutex_);
    game_users_[conn->GetId()] = gu;
}

void GateUserManager::RemoveUser(uint32 conn_id)
{
    Lock l(mutex_);
    auto it = game_users_.find(conn_id);
    if (it != game_users_.end()) {
        game_users_.erase(it);
        return;
    }
}

GateUserPtr GateUserManager::GetUser(uint32 conn_id)
{
    Lock l(mutex_);
    auto it = game_users_.find(conn_id);
    if (it != game_users_.end()) {
        return it->second;
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
