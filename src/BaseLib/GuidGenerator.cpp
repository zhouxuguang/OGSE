#include "GuidGenerator.h"
#include "Random.h"

NS_BASELIB_BEGIN

#define GUIDFormatString "%08X-%04X-%04X-%08X-%08X"

#define GUIDStringLength 36

#if !defined(WIN32) && !defined(_WIN64)

#ifndef __ANDROID__
    #include <uuid/uuid.h>
    #include <sys/time.h>
#endif

#elif defined WIN32

#include <sys/timeb.h>

#define snprintf sprintf_s  

#endif


class BASELIB_API_HIDE GUIDGenerator
{
public:
	static eUInt32 BytesToUInt32(const eByte bytes[]) 
	{
		return ((eUInt32) bytes[0]
		    | ((eUInt32) bytes[1] << 8)
			| ((eUInt32) bytes[2] << 16)
			| ((eUInt32) bytes[3] << 24));
	}
    
    static eUInt16 BytesToUInt16(const eByte bytes[])
    {
        return (eUInt16) bytes[0]
                | (eUInt16) bytes[1] << 8;
    }

	static void UInt32ToBytes(eByte bytes[], eUInt32 n) 
	{
		bytes[0] = n & 0xff;
		bytes[1] = (n >> 8) & 0xff;
		bytes[2] = (n >> 16) & 0xff;
		bytes[3] = (n >> 24) & 0xff;
	}

	static bool CreateGUID(GUID *guid) 
	{
        if (NULL == guid)
        {
            return false;
        }
        
        //使用真随机数接口实现
        guid->Data1 = GetRandom();
        guid->Data2 = (eUInt16)(GetRandom(0,65535));
        guid->Data3 = (eUInt16)(GetRandom(0,65535));
        UInt32ToBytes(&guid->Data4[0], GetRandom());
        UInt32ToBytes(&guid->Data4[4], GetRandom());
        return true;

	}
};

#ifdef WIN32

#include <Windows.h> 

bool CreateGUID(GUID *guid)
{
    if (NULL == guid)
    {
        return false;
    }
    
	return CoCreateGuid(guid) == S_OK;
}

#else
bool CreateGUID(GUID *guid)
{
    if (NULL == guid)
    {
        return false;
    }
    
#ifdef __ANDROID__
	return GUIDGenerator::CreateGUID(guid);
#else
    uuid_t uuid;
    uuid_generate_random(uuid);
    
    unsigned char *pUUID = uuid;
    
    guid->Data1 = GUIDGenerator::BytesToUInt32(pUUID);
    guid->Data2 = GUIDGenerator::BytesToUInt16(pUUID+4);
    guid->Data3 = GUIDGenerator::BytesToUInt16(pUUID+6);
    memcpy(guid->Data4, pUUID+8, 8 * sizeof(eByte));
    return true;
#endif
}

#endif

bool GUIDToString(const GUID *guid, std::string& bufStr)
{
    if (NULL == guid)
    {
        return false;
    }
    
	size_t nLen = GUIDStringLength;
	bufStr.resize(nLen+1);
	int num = sprintf((char *)bufStr.c_str(), GUIDFormatString,
		guid->Data1, guid->Data2, guid->Data3,
		GUIDGenerator::BytesToUInt32(&(guid->Data4[0])),
		GUIDGenerator::BytesToUInt32(&(guid->Data4[4])));

	if (num != GUIDStringLength)
		return false;

	bufStr[num] = '\0';
	return true;
}

bool IsGUIDEqual(const GUID &guid1,const GUID& guid2)
{
    return 0 == memcmp(&guid1, &guid2, sizeof(GUID));
}

NS_BASELIB_END

