#ifndef BASELIB_GUIDGENERATOR_INCLUDE_H_45FG6HJ
#define BASELIB_GUIDGENERATOR_INCLUDE_H_45FG6HJ

//GUID��������Windows�������еģ�����ƽ̨�Լ�ʵ��

#include "PreCompile.h" 

NS_BASELIB_BEGIN


#if !defined(WIN32) && !defined(_WIN64)
struct GUID
{
    eUInt32 Data1;
    eUInt16 Data2;
    eUInt16 Data3;
    eByte  Data4[8];
};
#endif


//����GUID
BASELIB_API bool CreateGUID(GUID *guid);

//GUID��ʽ��Ϊ�ַ���
BASELIB_API bool GUIDToString(const GUID *guid, std::string& buf);

BASELIB_API bool IsGUIDEqual(const GUID &guid1,const GUID& guid2);

NS_BASELIB_END

#endif
