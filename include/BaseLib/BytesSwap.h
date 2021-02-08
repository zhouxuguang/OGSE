
#ifndef BASELIB_BYTESWAP_INCLUDE_H_KG58GYUFT675JK
#define BASELIB_BYTESWAP_INCLUDE_H_KG58GYUFT675JK

//�ֽڽ�������Сβ���Լ��������ֽ���ʹ�Сβ��֮���ת���Ļ�����

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
    
    //32λ����ת��
    static eUInt32 SwapInt32LittleToHost(eUInt32 arg);
    
    static eUInt32 SwapInt32BigToHost(eUInt32 arg);
    
    static eUInt32 SwapInt32HostToLittle(eUInt32 arg);
    
    static eUInt32 SwapInt32HostToBig(eUInt32 arg);
    
    //16λ����ת��
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
