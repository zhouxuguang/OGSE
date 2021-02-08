#ifndef BASELIB_BYTEENCODE_INCLUDE_H_HJD458H
#define BASELIB_BYTEENCODE_INCLUDE_H_HJD458H

//字节序编码和解码的函数

#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API ByteEncode
{
public:
	ByteEncode(void);

	~ByteEncode(void);

	/*******************************************************************************
	@ 函数名称	: GetByteOrder
	@ 功能描述	: 判断机器的是大尾端还是小尾端
	@ 返 回 值	: int  1是小尾端，0是大尾端   pc一般是小尾端
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	eByte GetByteOrder();

	/*******************************************************************************
	@ 函数名称	: GetShort
	@ 功能描述	: 将字节流解析为短整型
	@ 参　　数	: const UBYTE* buf
	@ 参　　数	: UBYTE byteBoder
	@ 返 回 值	: short
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	eInt16 GetShort(const eByte* buf,eByte byteBoder);

	/*******************************************************************************
	@ 函数名称	: GetInt
	@ 功能描述	: 将字节流解析为整数
	@ 参　　数	: const UBYTE* buf
	@ 参　　数	: UBYTE byteBoder
	@ 返 回 值	: int
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	eInt32 GetInt(const eByte* buf,eByte byteBoder);

	/*******************************************************************************
	@ 函数名称	: PutInt
	@ 功能描述	: 将整型转化为字节数组
	@ 参　　数	: int intValue
	@ 参　　数	: UBYTE *buf
	@ 参　　数	: UBYTE byteOrder
	@ 返 回 值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	void PutInt(eInt32 intValue,eByte *buf,eByte byteOrder);

	/*******************************************************************************
	@ 函数名称	: GetLong
	@ 功能描述	: 将字节流转换为长整型
	@ 参　　数	: const UBYTE* buf
	@ 参　　数	: UBYTE byteBoder
	@ 返 回 值	: int64_t
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	eLonglong GetLong(const eByte* buf,eByte byteBoder);

	/*******************************************************************************
	@ 函数名称	: PutLong
	@ 功能描述	: 将长整型转化为字节数组
	@ 参　　数	: int64_t longValue
	@ 参　　数	: UBYTE *buf
	@ 参　　数	: UBYTE byteOrder
	@ 返 回 值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	void PutLong(eLonglong longValue,eByte *buf,eByte byteOrder);

	/*******************************************************************************
	@ 函数名称	: GetDouble
	@ 功能描述	: 将字节流解析为double型
	@ 参　　数	: const UBYTE* buf
	@ 参　　数	: UBYTE byteBoder
	@ 返 回 值	: double
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	double GetDouble(const eByte* buf,eByte byteBoder);

	/*******************************************************************************
	@ 函数名称	: PutDouble
	@ 功能描述	: 将double转化为字节数组
	@ 参　　数	: double doubleValue
	@ 参　　数	: UBYTE* buf
	@ 参　　数	: UBYTE byteOrder
	@ 返 回 值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	void PutDouble(double dbValue,eByte* buf,eByte byteOrder);

	float GetFloat(const eByte* buf,eByte byteBoder);

	void PutFloat(float fValue,eByte* buf,eByte byteOrder);

	/*******************************************************************************
	@ 函数名称	: ByteToString
	@ 功能描述	: 将字节数组转换为string
	@ 参　　数	: const UBYTE* buf
	@ 参　　数	: int len
	@ 返 回 值	: std::string
	@ 作　　者	: guang
	@ 设计日期	: 2013年1月12日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	std::string ByteToString(const eByte* buf,int nLen);
};

NS_BASELIB_END

#endif
