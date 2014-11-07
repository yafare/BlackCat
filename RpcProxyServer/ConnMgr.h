#ifndef _CONNMGR_H
#define _CONNMGR_H

#include <mutex>
#include <string>
#include <unordered_map>

#include "../ServerLib/ServerCommon.h"
#include "../ServerLib/TcpConnection.h"

class ConnMgr
{
public:
    void Add(const std::string& name, const ConnectionPtr& ptr)
    {
        Lock l(mutex_);
        auto it = conn_map_.find(name);
        if (it != conn_map_.end()) {
            printf("%s already login\n");
        }
        conn_map_[name] = ptr;
        name_map_[ptr->GetId()] = name;
    }

    std::string GetName(const ConnectionPtr& ptr)
    {
        Lock l(mutex_);
        auto it = name_map_.find(ptr->GetId());
        if (it == name_map_.end()) {
            return "";
        }
        return it->second;
    }

    ConnectionPtr GetConn(const std::string& name)
    {
        Lock l(mutex_);
        auto it = conn_map_.find(name);
        if (it == conn_map_.end()) {
            return 0;
        }
        return it->second;
    }

    void Del(const ConnectionPtr& ptr)
    {
        std::string name = GetName(ptr);
        if (!name.empty()) {
            Lock l(mutex_);
            name_map_.erase(ptr->GetId());
            conn_map_.erase(name);
        }
    }

private:
    std::mutex mutex_;
    std::unordered_map<uint32, std::string> name_map_;
    std::unordered_map<std::string, ConnectionPtr> conn_map_;
};
inline ConnMgr& GetConnMgr()
{
    static ConnMgr mgr;
    return mgr;
}

#endif // _CONNMGR_H
