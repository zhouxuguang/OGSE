#include "ByteEncode.h"

NS_BASELIB_BEGIN

const eByte ENDIAN_BIG = 0;		  //大尾端
const eByte ENDIAN_LITTLE = 1;	  //小尾端

const char hexDigits[] = {'0','1','2','3','4','5','6','7','8','9',
'A','B','C','D','E','F'};

ByteEncode::ByteEncode(void)
{
}

ByteEncode::~ByteEncode(void)
{
}

eByte ByteEncode::GetByteOrder()
{
	int tt = 1;
	char *c = (char*)(&tt);
	if(*c == 1)		//小尾端
	{
		return 0;
	}
	else
	{
		return 1;	//大尾端
	}
}

eInt16 ByteEncode::GetShort(const eByte* buf,eByte byteBoder)
{
	if (ENDIAN_BIG == byteBoder)	//大尾端
	{
		return (short)buf[0]<<8|
			(short)buf[1];
	}
	else
	{
		return (short)buf[1]<<8|
			(short)buf[0];
	}
}

eInt32 ByteEncode::GetInt(const eByte* buf,eByte byteBoder)
{
	if (ENDIAN_BIG == byteBoder)	//大尾端
	{
		return buf[0]<<24|
			buf[1]<<16|
			buf[2]<<8|
			buf[3];
	}
	else
	{
		return buf[3]<<24|
			buf[2]<<16|
			buf[1]<<8|
			buf[0];
	}
}

void ByteEncode::PutInt(eInt32 intValue,eByte *buf,eByte byteOrder)
{
	if (ENDIAN_BIG == byteOrder) //大尾端
	{
		buf[0] = (eByte)(intValue>>24);
		buf[1] = (eByte)(intValue>>16);
		buf[2] = (eByte)(intValue>>8);
		buf[3] = (eByte)(intValue);
	}
	else
	{
		buf[3] = (eByte)(intValue>>24);
		buf[2] = (eByte)(intValue>>16);
		buf[1] = (eByte)(intValue>>8);
		buf[0] = (eByte)(intValue);
	}
}

eLonglong ByteEncode::GetLong(const eByte* buf,eByte byteBoder)
{
	if (ENDIAN_BIG == byteBoder)	//大尾端
	{
		return (eLonglong)(buf[0])<<56|
			(eLonglong)(buf[1] & 0xff)<<48|
			(eLonglong)(buf[2] & 0xff)<<40|
			(eLonglong)(buf[3] & 0xff)<<32|
			(eLonglong)(buf[4] & 0xff)<<24|
			(eLonglong)(buf[5] & 0xff)<<16|
			(eLonglong)(buf[6] & 0xff)<<8|
			(eLonglong)(buf[7] & 0xff);
	}
	else
	{
		return (eLonglong)(buf[7])<<56|
			(eLonglong)(buf[6] & 0xff)<<48|
			(eLonglong)(buf[5] & 0xff)<<40|
			(eLonglong)(buf[4] & 0xff)<<32|
			(eLonglong)(buf[3] & 0xff)<<24|
			(eLonglong)(buf[2] & 0xff)<<16|
			(eLonglong)(buf[1] & 0xff)<<8|
			(eLonglong)(buf[0] & 0xff);
	}
}

void ByteEncode::PutLong(eLonglong longValue,eByte *buf,eByte byteOrder)
{
	if (ENDIAN_BIG == byteOrder)	//大尾端
	{
		buf[0] = (eByte)(longValue>>56);
		buf[1] = (eByte)(longValue>>48);
		buf[2] = (eByte)(longValue>>40);
		buf[3] = (eByte)(longValue>>32);
		buf[4] = (eByte)(longValue>>24);
		buf[5] = (eByte)(longValue>>16);
		buf[6] = (eByte)(longValue>>8);
		buf[7] = (eByte)(longValue);
	}
	else
	{
		buf[7] = (eByte)(longValue>>56);
		buf[6] = (eByte)(longValue>>48);
		buf[5] = (eByte)(longValue>>40);
		buf[4] = (eByte)(longValue>>32);
		buf[3] = (eByte)(longValue>>24);
		buf[2] = (eByte)(longValue>>16);
		buf[1] = (eByte)(longValue>>8);
		buf[0] = (eByte)(longValue);
	}
}

double ByteEncode::GetDouble(const eByte* buf,eByte byteBoder)
{
	eLonglong longValue = GetLong(buf,byteBoder);
	double ret;
	memcpy(&ret,&longValue,sizeof(double));
	return ret;
}

void ByteEncode::PutDouble(double doubleValue,eByte* buf,eByte byteOrder)
{
	eLonglong longValue = 0;
	memcpy(&longValue,&doubleValue,sizeof(double));
	PutLong(longValue,buf,byteOrder);
}

std::string ByteEncode::ByteToString(const eByte* buf,int nLen)
{
	std::string str;
	for (register int i = 0; i < nLen; i ++)
	{
		eByte byte = buf[i];
		str += std::string(1,hexDigits[(byte>>4) & 0xff]);
		str += std::string(1,hexDigits[byte & 0xff]);
	}
	return str;
}

float ByteEncode::GetFloat(const eByte* buf,eByte byteBoder)
{
	eInt32 longValue = GetInt(buf,byteBoder);
	float ret = 0;
	memcpy(&ret,&longValue,sizeof(float));
	return ret;
}

void ByteEncode::PutFloat(float dbValue,eByte* buf,eByte byteOrder)
{
	eInt32 nValue = 0;
	memcpy(&nValue,&dbValue,sizeof(float));
	PutInt(nValue,buf,byteOrder);
}

NS_BASELIB_END
