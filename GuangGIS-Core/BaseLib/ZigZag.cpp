#include "ZigZag.h"

NS_BASELIB_BEGIN

eInt32 ZagEncode(eUInt32 nZigValue)
{
	eInt32 nValue = nZigValue;
	return (-(nValue & 0x01)) ^ ((nValue >> 1) & ~(1 << 31));
}

eUInt32 ZigDecode(eInt32 nValue)
{
	return (eUInt32)((nValue << 1) ^ (nValue >> 31) );
}

NS_BASELIB_END
