#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include <memory>
#include <functional>
#include <unordered_map>

#include "ServerCommon.h"

#include "../Common/Types.h"

class BasePacketCallback
{
public:
    virtual ~BasePacketCallback() {}
    virtual void OnMessage(const ConnectionPtr&, ProtobufMessage *msg) = 0;
};

template <typename T>
class PacketCallbackT : public BasePacketCallback
{
public:
    using MessageCallback = std::function<void(const ConnectionPtr&, const T&)>;
    PacketCallbackT(const MessageCallback& cb) : cb_(cb)
    {
    }

    virtual void OnMessage(const ConnectionPtr& conn, ProtobufMessage *msg)
    {
        T *t = dynamic_cast<T *>(msg);
        cb_(conn, *t);
    }

private:
    MessageCallback cb_;
};

static void DefaultMessageHandler(const ConnectionPtr& conn, ProtobufMessage* message)
{
}

inline uint64 GetIdFromPtr(const void *ptr)
{
    return reinterpret_cast<uint64>(ptr);
}

class PacketDispatcher
{
    using DefaultMessageHandlerFunc = std::function<void(const ConnectionPtr&, ProtobufMessage* message)>;
public:
    PacketDispatcher() : default_callback_(DefaultMessageHandler)
    {
    }

    PacketDispatcher(const DefaultMessageHandlerFunc& func) :default_callback_(func)
    {
    }

public:
    bool HaveCompleteMessage(const uint8 *buf, int32 len)
    {
        if (len < kHeaderLen) {
            return false;
        }

        int32 cur_len = ToInt32(buf);
        if (static_cast<int>(len) < cur_len + kHeaderLen) {
            return false;
        }
        return true;
    }

    std::string GetMessage(const uint8 *buf, int32 len)
    {
        std::string result;

        int32 cur_len = ToInt32(buf);
        result.resize(cur_len);
        std::copy(buf + kHeaderLen, buf + cur_len + kHeaderLen, result.begin());

        return result;
    }

    void OnMessage(const ConnectionPtr& conn, ProtobufMessage *message)
    {
        auto it = callbacks_.find(GetIdFromPtr(message->GetDescriptor()));
        if (it != callbacks_.end()) {
            it->second->OnMessage(conn, message);
        } else {
            default_callback_(conn, message);
        }
    }

    template<typename T>
    void RegisterMessageCallback(const typename PacketCallbackT<T>::MessageCallback& cb)
    {
        std::shared_ptr<PacketCallbackT<T>> pd(new PacketCallbackT<T>(cb));
        callbacks_[GetIdFromPtr(T::descriptor())] = pd;
    }

private:
    std::unordered_map<uint64, std::shared_ptr<BasePacketCallback>> callbacks_;
    DefaultMessageHandlerFunc default_callback_;
};

#endif // _DISPATCHER_H
