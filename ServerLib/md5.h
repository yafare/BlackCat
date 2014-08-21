#ifndef _MD5_H
#define _MD5_H

typedef unsigned int UWORD32;

#define md5byte unsigned char

struct MD5Context {
    UWORD32 buf[4];
    UWORD32 bytes[2];
    UWORD32 in[16];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, md5byte const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(UWORD32 buf[4], UWORD32 const in[16]);

#include <string>
#include <algorithm>

inline std::string GetMD5(md5byte *buf, int len)
{
    MD5Context context;
    MD5Init(&context);
    MD5Update(&context, buf, len);

    md5byte digest[16] = { 0 };
    MD5Final(digest, &context);

    std::string str;
    str.resize(sizeof(digest));
    memcpy((void *)&str[0], digest, sizeof(digest));
    return str;
}

#endif  // _MD5_H
