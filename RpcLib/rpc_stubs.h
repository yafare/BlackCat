#ifndef _RPC_STUBS_H
#define _RPC_STUBS_H

#include <memory>
#include <string>
#include <functional>

#include "RpcServer.pb.h"

#include "../Common/Types.h"
#include "../ServerLib/Codec.h"

namespace rpclib
{

struct RpcResult
{
    std::string name;
    std::string body;
};
template <typename T>
inline RpcResult MakeRpcResult(const T& t)
{
    return{ t.GetTypeName(), t.SerializeAsString() };
}

class RpcCallback
{
public:
    virtual ~RpcCallback()
    {
    }
    virtual RpcResult Execute(const RpcServer::RpcRequest& args) = 0;
};
using RpcCallbackPtr = std::shared_ptr<RpcCallback>;

template <typename T>
inline RpcCallbackPtr MakeCallback(const std::function<RpcResult(const T&)>& func)
{
    class Callback : public RpcCallback
    {
        using CallbackFunc = std::function<RpcResult(const T&)>;
    public:
        Callback(const CallbackFunc& func) : func_(func)
        {
        }
        virtual RpcResult Execute(const RpcServer::RpcRequest& args)
        {
            auto message = CreateMessage(args.request_name());
            if (message != 0 && message->ParseFromString(args.request_body())) {
                RpcResult result = func_(dynamic_cast<const T&>(*message));
                delete message;
                return result;
            }

            // TODO
            return RpcResult();
        }

    private:
        CallbackFunc func_;
    };
    return RpcCallbackPtr(new Callback(func));
}

}

#endif  // _RPC_STUBS_H
