#ifndef _INDEXMGR_H
#define _INDEXMGR_H

#include <mutex>
#include <unordered_set>

#include "ServerCommon.h"

template <typename T>
class IndexMgr
{
public:
    IndexMgr() : id_(0)
    {
    }

public:
    T GetId()
    {
        Lock l(id_mutex_);
        while (IdExist(id_)) {
            id_++;
        }
        id_set_.insert(id_);
        return id_++;
    }

    void PutId(T id)
    {
        Lock l(id_mutex_);
        auto it = id_set_.find(id);
        if (it != id_set_.end()) {
            id_set_.erase(it);
        }
    }

private:
    bool IdExist(T id)
    {
        auto it = id_set_.find(id);
        if (it == id_set_.end()) {
            return false;
        }
        return true;
    }

private:
    T                           id_;
    std::unordered_set<T>       id_set_;
    std::mutex                  id_mutex_;
};

#endif // !_INDEXMGR_H
