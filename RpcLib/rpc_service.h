#ifndef _RPC_SERVICE_H
#define _RPC_SERVICE_H

#include <thread>
#include <string>
#include <map>
#include <mutex>
#include <unordered_map>
#include <functional>

#include "rpc_task.h"
#include "rpc_stubs.h"

#include "RpcServer.pb.h"

#include "../Common/Types.h"
#include "../ServerLib/strop.h"
#include "../ServerLib/TcpClient.h"
#include "../ServerLib/Dispatcher.h"

namespace rpclib
{

class RpcServiceProvider : public std::enable_shared_from_this<RpcServiceProvider>
{
    using ResultCallback = std::function<void(const RpcResult&)>;
    using ResultCallbackPtr = std::shared_ptr<ResultCallback>;

public:
    RpcServiceProvider(const std::string& name) :
        name_(name),
        dispatcher_(std::bind(&RpcServiceProvider::DefaultHandler, this, std::placeholders::_1, std::placeholders::_2))
    {
        tcp_client_.reset(new TcpClient(ios_));
        tcp_client_->SetCallBacks(GetCallBacks());

        RegisterHanlders();

        task_runner_ = std::thread(std::bind(&RpcTaskMgr::Run, &task_mgr_));
        network_runner_ = std::thread(std::bind(&RpcServiceProvider::IosRun, this));
    }
    ~RpcServiceProvider()
    {
        ios_.stop();
        task_mgr_.Stop();
        task_runner_.join();
        network_runner_.join();
    }

public:
    bool Connect(const std::string& rpc_server)
    {
        auto vec = SplitString(rpc_server, ':');
        if (vec.size() < 2) {
            return false;
        }
        tcp_client_->Connect(vec[0], vec[1]);
        return true;
    }

    void Stop()
    {
        task_mgr_.Stop();
    }

public:
    template <typename T>
    RpcServiceProvider& Register(const std::function<RpcResult(const T&)>& func)
    {
        function_map_[T().GetTypeName()] = MakeCallback(func);
        return *this;
    }

    template <typename T>
    void Call(const std::string& service, const T& req, const ResultCallback& cb)
    {
        ResultCallbackPtr ptr(new ResultCallback(cb));
        uint64 cb_id = GetIdFromPtr(ptr.get());
        {
            Lock l(mutex_);
            callback_map_[cb_id] = ptr;
        }
        SendRequestToServer(service, req, cb_id);
    }

private:
    template <typename T>
    void SendRequestToServer(const std::string& service, const T& req, uint64 cb_id)
    {
        RpcServer::RpcRequest request;
        request.set_service_name(service);
        request.set_requestor(name_);
        request.set_callback_id(cb_id);
        request.set_request_name(req.GetTypeName());
        request.set_request_body(req.SerializeAsString());
        Send(request);
    }

    void Dispatch(const std::string& pkt)
    {
        auto msg = Decode(pkt);
        if (msg == 0) {
            return;
        }
        dispatcher_.OnMessage(tcp_client_, msg);
        delete msg;
    }

private:
    // Unknown packet
    void DefaultHandler(const ConnectionPtr&, ProtobufMessage *msg)
    {
    }
    // RpcServer.RpcRequest handler
    void HandleRpcRequest(const ConnectionPtr&, const RpcServer::RpcRequest& request)
    {
        if (request.service_name() != name_) {
            return;
        }

        auto handler = [&](const RpcServer::RpcRequest& request) {
            RpcServer::RpcResponse response;
            response.set_service_name(request.service_name());
            response.set_requestor(request.requestor());
            response.set_callback_id(request.callback_id());

            auto func = GetCallbackFromName(request.request_name());
            if (func == 0) {
                std::ostringstream ost;
                ost << request.service_name() << " 上不提供 " << request.request_name()
                    << " 服务";

                RpcServer::RpcError err;
                err.set_error(ost.str());

                response.set_response_name(err.GetTypeName());
                response.set_response_body(err.SerializeAsString());
            } else {
                auto result = func->Execute(request);

                response.set_response_name(result.name);
                response.set_response_body(result.body);
            }

            Send(response);
        };
        task_mgr_.Push(rpclib::RpcTask(std::bind(handler, request)));
    }
    // RpcServer.RpcResponse handler
    void HandleRpcResponse(const ConnectionPtr&, const RpcServer::RpcResponse& response)
    {
        auto func = GetCallbackFromId(response.callback_id());
        if (func == 0) {
            // TODO: log it
            printf("invalid callback id\n");
            return;
        }
        RemoveCallbackFromId(response.callback_id());

        RpcResult result = { response.response_name(), response.response_body() };
        (*func)(result);
    }
    void RegisterHanlders()
    {
#define REG_FUNC(C) \
    dispatcher_.RegisterMessageCallback<RpcServer::C>(std::bind(&RpcServiceProvider::Handle##C, \
        this, std::placeholders::_1, std::placeholders::_2));

        REG_FUNC(RpcRequest);
        REG_FUNC(RpcResponse);

#undef REG_FUNC
    }
    void IosRun()
    {
        boost::asio::io_service::work work(ios_);
        ios_.run();
    }

private:
    // call backs
    void OnConnected(const ConnectionPtr& conn, bool success)
    {
        if (success) {
            RpcServer::LoginRequest req;
            req.set_service_name(name_);
            req.set_secure_key("test key");
            Send(req);
        }
    }
    int32 OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len)
    {
        int32 total_len = 0;

        do {
            if (!dispatcher_.HaveCompleteMessage(buf, len)) {
                break;
            }
            std::string msg = dispatcher_.GetMessage(buf, len);
            Dispatch(msg);
            int32 cur_size = static_cast<int32>(msg.size()) + kHeaderLen;
            buf += cur_size;
            len -= cur_size;
            total_len += cur_size;
        } while (true);

        return total_len;
    }
    void OnWrite(const ConnectionPtr& conn, int32 len)
    {
    }
    void OnDisconnect(const ConnectionPtr& conn)
    {
    }
    ConnectionCallBacks GetCallBacks()
    {
        ConnectionCallBacks cb = {
            std::bind(&RpcServiceProvider::OnConnected, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&RpcServiceProvider::OnRead, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&RpcServiceProvider::OnWrite, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&RpcServiceProvider::OnDisconnect, this, std::placeholders::_1),
        };
        return cb;
    }
    template <typename T>
    void Send(const T& pkt)
    {
        std::string s = Encode(pkt);
        if (s.empty()) {
            return;
        }
        tcp_client_->Send((const uint8 *)s.c_str(), static_cast<int32>(s.size()));
    }

private:
    RpcCallbackPtr GetCallbackFromName(const std::string name)
    {
        auto it = function_map_.find(name);
        return (it == function_map_.end()) ? 0 : it->second;
    }
    ResultCallbackPtr GetCallbackFromId(uint64 id)
    {
        Lock l(mutex_);
        auto it = callback_map_.find(id);
        if (it == callback_map_.end()) {
            return 0;
        }
        return it->second;
    }
    void RemoveCallbackFromId(uint64 id)
    {
        Lock l(mutex_);
        callback_map_.erase(id);
    }

private:
    std::unordered_map<std::string, RpcCallbackPtr> function_map_;
    std::unordered_map<uint64, ResultCallbackPtr> callback_map_;

    std::mutex          mutex_;

    std::string         name_;
    RpcTaskMgr          task_mgr_;
    std::thread         task_runner_;
    std::thread         network_runner_;

    TcpClientPtr        tcp_client_;
    PacketDispatcher    dispatcher_;
    IoService           ios_;
};

}

#endif  // _RPC_SERVICE_H
