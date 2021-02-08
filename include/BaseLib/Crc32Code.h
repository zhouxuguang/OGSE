#ifndef CRC32CODE_BASELIB_INCLUDE_H
#define CRC32CODE_BASELIB_INCLUDE_H

#include "PreCompile.h"

NS_BASELIB_BEGIN

BASELIB_API eUInt32 Calculate_CRC32(const void *pStart, eUInt32 uSize);

BASELIB_API eUInt16 Calculate_CRC16(const void* pStart, eUInt32 uSize);

NS_BASELIB_END

#endif
