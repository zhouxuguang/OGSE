#include "BaseLib/BytesSwap.h"

NS_BASELIB_BEGIN


#if defined(WIN32) && defined(_MSC_VER) && (_MSC_VER > 1298)
	#include <stdlib.h>
	#define bswap_16(X) _byteswap_ushort(X)
	#define bswap_32(X) _byteswap_ulong(X)
#if (_MSC_VER >= 1400)
	#define localtime _localtime_s
#endif
#elif defined(__APPLE__) || defined(MACOSX)
	#include <libkern/OSByteOrder.h>
	#define bswap_16(X) OSReadSwapInt16(&X,0)
	#define bswap_32(X) OSReadSwapInt32(&X,0)
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
	#include <sys/endian.h>
	#define bswap_16(X) bswap16(X)
	#define bswap_32(X) bswap32(X)
#elif defined(__ANDROID__) || defined(__IOS__) || defined(__linux__)
	#include <byteswap.h>
#else
	#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&0xFF00) >> 8))
	#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
#endif


#define bswap_64_ptr(x) \
{                                                                 \
	eByte       byTemp, *_pabyDataT = (eByte *) (x);              \
	\
	byTemp = _pabyDataT[0];                                       \
	_pabyDataT[0] = _pabyDataT[7];                                \
	_pabyDataT[7] = byTemp;                                       \
	byTemp = _pabyDataT[1];                                       \
	_pabyDataT[1] = _pabyDataT[6];                                \
	_pabyDataT[6] = byTemp;                                       \
	byTemp = _pabyDataT[2];                                       \
	_pabyDataT[2] = _pabyDataT[5];                                \
	_pabyDataT[5] = byTemp;                                       \
	byTemp = _pabyDataT[3];                                       \
	_pabyDataT[3] = _pabyDataT[4];                                \
	_pabyDataT[4] = byTemp;                                       \
}

#define bswap_32f(x) \
{                                                                 \
	eByte       byTemp, *_pabyDataT = (eByte *) (x);              \
	\
	byTemp = _pabyDataT[0];                                       \
	_pabyDataT[0] = _pabyDataT[3];                                \
	_pabyDataT[3] = byTemp;                                       \
	byTemp = _pabyDataT[1];                                       \
	_pabyDataT[1] = _pabyDataT[2];                                \
	_pabyDataT[2] = byTemp;                                       \
} 

static inline eUInt32 swap_long(eUInt32 v)
{
    return (v<<24) | ((v<<8)&0x00FF0000) | ((v>>8)&0x0000FF00) | (v>>24);
}

static inline eUInt16 swap_short(eUInt16 v)
{
    return (v<<8) | (v>>8);
}

eUInt16 BytesSwap::SwapBytes(eUInt16 num)
{
	return bswap_16(num);
}

eInt16 BytesSwap::SwapBytes(eInt16 num)
{
	return bswap_16(num);
}

eUInt32 BytesSwap::SwapBytes(eUInt32 num)
{
	return bswap_32(num);
}

eInt32 BytesSwap::SwapBytes(eInt32 num)
{
	return bswap_32(num);
}


eDouble BytesSwap::SwapBytes(eDouble num)
{
	bswap_64_ptr(&num);
	return num;
}

eFloat BytesSwap::SwapBytes(eFloat num)
{
	bswap_32f(&num);
	return num;
}

inline bool GetByteOrder()
{
    union
    {
        char big_endian_1[2];
        eInt16 is_it_really_1;
    } u;
    u.big_endian_1[0] = 0;
    u.big_endian_1[1] = 1;

    if (u.is_it_really_1 == 1)
        return 1;//大尾端
    else
        return 0;
}

static bool g_bBigEndian = GetByteOrder();

eUInt32 BytesSwap::SwapInt32LittleToHost(eUInt32 arg)
{
    if (g_bBigEndian)
    {
        return SwapBytes(arg);
    }
    
    else
    {
        return arg;
    }
}

eUInt32 BytesSwap::SwapInt32BigToHost(eUInt32 arg)
{
    if (g_bBigEndian)
    {
        return arg;
    }
    
    else
    {
        return SwapBytes(arg);
    }
}

eUInt32 BytesSwap::SwapInt32HostToLittle(eUInt32 arg)
{
    if (g_bBigEndian)
    {
        return SwapBytes(arg);
    }
    
    else
    {
        return arg;
    }
}

eUInt32 BytesSwap::SwapInt32HostToBig(eUInt32 arg)
{
    if (g_bBigEndian)
    {
        return arg;
    }
    
    else
    {
        return SwapBytes(arg);
    }
}

eUInt16 BytesSwap::SwapInt16LittleToHost(eUInt16 arg)
{
    if (g_bBigEndian)
    {
        return SwapBytes(arg);
    }
    
    else
    {
        return arg;
    }
}

eUInt16 BytesSwap::SwapInt16BigToHost(eUInt16 arg)
{
    if (g_bBigEndian)
    {
        return arg;
    }
    
    else
    {
        return SwapBytes(arg);
    }
}

eUInt16 BytesSwap::SwapInt16HostToLittle(eUInt16 arg)
{
    if (g_bBigEndian)
    {
        return SwapBytes(arg);
    }
    
    else
    {
        return arg;
    }
}

eUInt16 BytesSwap::SwapInt16HostToBig(eUInt16 arg)
{
    if (g_bBigEndian)
    {
        return arg;
    }
    
    else
    {
        return SwapBytes(arg);
    }
}

NS_BASELIB_END
