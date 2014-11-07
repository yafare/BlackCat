#ifndef _CODEC_H
#define _CODEC_H

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "../Common/Types.h"

using ProtobufMessage = google::protobuf::Message;

// struct ProtobufTransportFormat __attribute__ ((__packed__))
// {
//   int32      len;
//   int32      name_len;
//   char       type_name[nameLen];
//   char       protobuf_data[len-nameLen-8];
// }

const int32 kHeaderLen = sizeof(int32);

///
/// Encode protobuf Message to transport format defined above
/// returns a std::string.
///
/// returns a empty string if message.AppendToString() fails.
///
inline std::string Encode(const ProtobufMessage& message)
{
    std::string result;

    result.resize(kHeaderLen);

    const std::string& type_name = message.GetTypeName();
    int32 name_len = static_cast<int32>(type_name.size() + 1);
    result.append(reinterpret_cast<char *>(&name_len), sizeof(name_len));
    result.append(type_name.c_str(), name_len);
    bool succeed = message.AppendToString(&result);

    if (succeed) {
        int32 len = result.size() - kHeaderLen;
        std::copy(reinterpret_cast<char *>(&len),
            reinterpret_cast<char *>(&len) + sizeof(len), result.begin());
    } else {
        result.clear();
    }

    return result;
}

inline ProtobufMessage *CreateMessage(const std::string& type_name)
{
    ProtobufMessage *message = 0;
    const auto descriptor =
        google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
    if (descriptor) {
        const auto prototype =
            google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype) {
            message = prototype->New();
        }
    }
    return message;
}

inline int32 ToInt32(const void *buf)
{
    int32 be32 = 0;
    memcpy(&be32, buf, sizeof(be32));
    return be32;
}

///
/// Decode protobuf Message from transport format defined above.
/// returns a Message *
///
/// returns 0 if fails.
///
inline ProtobufMessage *Decode(const std::string& buf)
{
    ProtobufMessage *result = 0;

    int32 len = static_cast<int32>(buf.size());
    if (len >= 10) {
        int32 name_len = ToInt32(buf.c_str());
        if (name_len >= 2 && name_len <= len - kHeaderLen) {
            std::string type_name(buf.begin() + kHeaderLen, buf.begin() + kHeaderLen + name_len - 1);
            ProtobufMessage *message = CreateMessage(type_name);
            if (message) {
                const char* data = buf.c_str() + kHeaderLen + name_len;
                int32 data_len = len - name_len - kHeaderLen;
                if (message->ParseFromArray(data, data_len)) {
                    result = message;
                } else {
                    // parse error
                    delete message;
                }
            }
        }
    }

    return result;
}

#endif // _CODEC_H
