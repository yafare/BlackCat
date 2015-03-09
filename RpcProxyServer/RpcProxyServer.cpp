#include "RpcProxyServer.h"
#include "ConnMgr.h"

#include "../ServerLib/TcpServer.h"
#include "../ServerLib/TcpConnection.h"

RpcProxyServer::RpcProxyServer()
{
#define REG_FUNC(C) \
    dispatcher_.RegisterMessageCallback<ConnectionPtr, RpcServer::C>(std::bind(&RpcProxyServer::Handle##C, \
        this, std::placeholders::_1, std::placeholders::_2));

    REG_FUNC(LoginRequest);
    REG_FUNC(RpcRequest);
    REG_FUNC(RpcResponse);

#undef REG_FUNC
}

void RpcProxyServer::HandleLoginRequest(const ConnectionPtr& conn, const RpcServer::LoginRequest& request)
{
    std::string service_name = request.service_name();
    printf("%s request to login\n", service_name.c_str());
    GetConnMgr().Add(service_name, conn);
}

void RpcProxyServer::HandleRpcRequest(const ConnectionPtr& conn, const RpcServer::RpcRequest& request)
{
    printf("RPC request: %s => %s for %s\n", request.requestor().c_str(), request.service_name().c_str(), request.request_name().c_str());
    auto dst_ptr = GetConnMgr().GetConn(request.service_name());
    if (dst_ptr == 0) {
        RpcServer::RpcError err;
        std::ostringstream ost;
        ost << request.service_name() << " is not online now";
        err.set_error(ost.str());

        RpcServer::RpcResponse response;
        response.set_service_name(request.service_name());
        response.set_requestor(request.requestor());
        response.set_callback_id(request.callback_id());
        response.set_response_name(err.GetTypeName());
        response.set_response_body(err.SerializeAsString());
        Send(conn, response);
        return;
    }
    Send(dst_ptr, request);
}

void RpcProxyServer::HandleRpcResponse(const ConnectionPtr& conn, const RpcServer::RpcResponse& response)
{
    printf("RPC response: %s => %s for %s\n", response.service_name().c_str(), response.requestor().c_str(), response.response_name().c_str());
    auto dst_ptr = GetConnMgr().GetConn(response.requestor());
    if (dst_ptr == 0) {
        // TODO: log it
        return;
    }
    Send(dst_ptr, response);
}

void RpcProxyServer::Dispatch(const ConnectionPtr& conn, const std::string& buf)
{
    auto msg = Decode(buf);
    if (msg == 0) {
        return;
    }
    dispatcher_.OnMessage(conn, msg);
    delete msg;
}

int32 RpcProxyServer::OnRead(const ConnectionPtr& conn, const uint8 *buf, int32 len)
{
    if (len >= MAX_RECV_BUF) {
        LOG("Recv buffer overflow, shutdown this connection");
        conn->Shutdown();
        return len;
    }

    int32 total_len = 0;

    do {
        if (!dispatcher_.HaveCompleteMessage(buf, len)) {
            break;
        }
        std::string msg = dispatcher_.GetMessage(buf, len);
        Dispatch(conn, msg);
        int32 cur_size = static_cast<int32>(msg.size()) + kHeaderLen;
        buf += cur_size;
        len -= cur_size;
        total_len += cur_size;
    } while (true);

    return total_len;
}

void RpcProxyServer::OnDisconnect(const ConnectionPtr& conn)
{
    std::string name = GetConnMgr().GetName(conn);
    if (!name.empty()) {
        printf("%s disconnect\n", name.c_str());
    }
    GetConnMgr().Del(conn);
}

void RpcProxyServer::HandlePacket(const ConnectionPtr& conn, const ProtobufMessage& packet)
{

}
