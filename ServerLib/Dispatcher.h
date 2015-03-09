#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include <memory>
#include <functional>
#include <unordered_map>

#include "ServerCommon.h"

#include "../Common/Types.h"

template <typename T>
class BasePacketCallback
{
public:
    virtual ~BasePacketCallback() {}
    virtual void OnMessage(const T&, ProtobufMessage *msg) = 0;
};

template <typename T1, typename T2>
class PacketCallbackT : public BasePacketCallback<T1>
{
public:
    using MessageCallback = std::function<void(const T1&, const T2&)>;
    PacketCallbackT(const MessageCallback& cb) : cb_(cb)
    {
    }

    virtual void OnMessage(const T1& t1, ProtobufMessage *msg)
    {
        T2 *t2 = dynamic_cast<T2 *>(msg);
        cb_(t1, *t2);
    }

private:
    MessageCallback cb_;
};

inline uint64 GetIdFromPtr(const void *ptr)
{
    return reinterpret_cast<uint64>(ptr);
}

template <typename T1, typename T2>
class PacketDispatcher
{
    static void DefaultMessageHandler(const T1& t1, T2 *t2)
    {
    }

    using DefaultMessageHandlerFunc = std::function<void(const T1&, T2 *message)>;
public:
    PacketDispatcher() : default_callback_(DefaultMessageHandler)
    {
    }

    PacketDispatcher(const DefaultMessageHandlerFunc& func) : default_callback_(func)
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

    void OnMessage(const T1& conn, T2 *message)
    {
        auto it = callbacks_.find(GetIdFromPtr(message->GetDescriptor()));
        if (it != callbacks_.end()) {
            it->second->OnMessage(conn, message);
        } else {
            default_callback_(conn, message);
        }
    }

    template <typename T1, typename T2>
    void RegisterMessageCallback(const typename PacketCallbackT<T1, T2>::MessageCallback& cb)
    {
        std::shared_ptr<PacketCallbackT<T1, T2>> pd(new PacketCallbackT<T1, T2>(cb));
        callbacks_[GetIdFromPtr(T2::descriptor())] = pd;
    }

private:
    std::unordered_map<uint64, std::shared_ptr<BasePacketCallback<T1>>> callbacks_;
    DefaultMessageHandlerFunc default_callback_;
};

#endif // _DISPATCHER_H
