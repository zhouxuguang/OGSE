
#ifndef BASELIB_BYTESWAP_INCLUDE_H_KG58GYUFT675JK
#define BASELIB_BYTESWAP_INCLUDE_H_KG58GYUFT675JK

//字节交换、大小尾端以及机器的字节序和大小尾端之间的转换的基础类

#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API BytesSwap
{
public:
	static eUInt16 SwapBytes(eUInt16 num);

	static eInt16 SwapBytes(eInt16 num);

	static eUInt32 SwapBytes(eUInt32 num);

	static eInt32 SwapBytes(eInt32 num);

	static eFloat SwapBytes(eFloat num);

	static eDouble SwapBytes(eDouble num);
    
    //32位整数转换
    static eUInt32 SwapInt32LittleToHost(eUInt32 arg);
    
    static eUInt32 SwapInt32BigToHost(eUInt32 arg);
    
    static eUInt32 SwapInt32HostToLittle(eUInt32 arg);
    
    static eUInt32 SwapInt32HostToBig(eUInt32 arg);
    
    //16位整数转换
    static eUInt16 SwapInt16LittleToHost(eUInt16 arg);
    
    static eUInt16 SwapInt16BigToHost(eUInt16 arg);
    
    static eUInt16 SwapInt16HostToLittle(eUInt16 arg);
    
    static eUInt16 SwapInt16HostToBig(eUInt16 arg);
    
private:
    BytesSwap();
    ~BytesSwap();
    BytesSwap(const BytesSwap&);
    BytesSwap& operator = (const BytesSwap&);
};

NS_BASELIB_END

#endif
