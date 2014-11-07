#ifndef _RPC_TASK_H
#define _RPC_TASK_H

#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace rpclib
{

class RpcTask
{
    using RpcTaskFunc = std::function<void()>;

public:
    RpcTask()
    {
    }

    RpcTask(const RpcTaskFunc& func) : func_(func)
    {
    }

    operator bool()
    {
        return func_ != 0;
    }

public:
    void Run()
    {
        if (func_ != 0) {
            func_();
        }
    }

private:
    RpcTaskFunc func_;
};

class RpcTaskMgr
{
    using Lock = std::unique_lock<std::mutex>;

public:
    RpcTaskMgr() : running_(true)
    {
    }

    ~RpcTaskMgr()
    {
    }

public:
    void Push(const RpcTask& task)
    {
        Lock l(mutex_);
        tasks_.push(task);
        cv_.notify_all();
    }

    void Run()
    {
        RpcTask t;
        while (Pop(t)) {
            t.Run();
        }
    }

    void Stop()
    {
        Lock l(mutex_);
        running_ = false;
        while (!tasks_.empty()) {
            tasks_.pop();
        }
        cv_.notify_all();
    }

private:
    bool Pop(RpcTask& t)
    {
        Lock l(mutex_);
        while (tasks_.empty()) {
            if (!running_) {
                return false;
            }
            cv_.wait(l);
        }
        t = tasks_.front();
        tasks_.pop();
        return running_;
    }

private:
    std::mutex                  mutex_;
    std::condition_variable     cv_;
    std::queue<RpcTask>         tasks_;
    bool                        running_;
};

}

#endif  // _RPC_TASK_H
