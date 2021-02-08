#ifndef REFERENCE_INCLUDE
#define REFERENCE_INCLUDE

//���ü������ʵ��

#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API Reference
{
public:
	Reference();

    ~Reference();
	
    //�������ø���
	eUInt32 ReleaseReference();
	
    //�������ø���
	eUInt32 AddReference();
    
    //������ø���
    eUInt32 GetReferenceCount() const;

private:
    //���������4�ֽ�����
	volatile AlignedUint32 m_nRef;
};

NS_BASELIB_END

#endif
