#ifndef BASELIB_HASHFUNCTION_INCLUDE_H
#define BASELIB_HASHFUNCTION_INCLUDE_H

#include "PreCompile.h"

NS_BASELIB_BEGIN

BASELIB_API eUInt32 HashFunction(const void* pKey,int nLen);

BASELIB_API eUInt32 GetHashCode(const std::string& key);

template<typename KEY>
eUInt32 GetHashCode(const KEY& key)
{
	return HashFunction(&key,sizeof(key));
}

NS_BASELIB_END

#endif
