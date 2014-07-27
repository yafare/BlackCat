#ifndef _TYPES_H
#define _TYPES_H

#define _USE_STDINT 0

#if _USE_STDINT

    #include <stdint.h>

    typedef uint8_t     byte;
    typedef uint8_t     uint8;
    typedef uint16_t    uint16;
    typedef uint32_t    uint32;
    typedef uint64_t    uint64;
    typedef int8_t      int8;
    typedef int16_t     int16;
    typedef int32_t     int32;
    typedef int64_t     int64;

#else

    typedef unsigned char   byte;
    typedef unsigned char   uint8;
    typedef signed char     int8;
    typedef unsigned short  uint16;
    typedef signed short    int16;
    typedef unsigned int    uint32;
    typedef signed int      int32;

    #ifdef _MSC_VER
        #if defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64
            typedef unsigned __int64    uint64;
            typedef unsigned __int64    uint64;
            typedef signed __int64      int64;
        #else
            #error __int64 not supported
        #endif
    #else
        typedef unsigned long long      uint64;
        typedef unsigned long long      uint64;
        typedef signed long long        int64;
    #endif

#endif

#endif  // _TYPES_H
