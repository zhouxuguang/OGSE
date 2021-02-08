/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoRasterDefine.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月23日
@ 内容摘要	: GDAL栅格数据的基本定义
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/

#ifndef __GEORASTERDEFINE_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEORASTERDEFINE_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include "GeoDefine.h"

//像素的数据类型
typedef enum 
{
	/*! 未知类型 */ 							PDT_Unknown = 0,
	/*! 8位无符号型 */							PDT_Byte = 1,
	/*! 16位无符号型 */							PDT_UInt16 = 2,
	/*! 16位有符号型 */						    PDT_Int16 = 3,
	/*! 32位无符号整型 */						PDT_UInt32 = 4,
	/*! 32位有符号整型 */						PDT_Int32 = 5,
	/*! 32位浮点型 */							PDT_Float32 = 6,
	/*! 64位浮点型 */							PDT_Float64 = 7,
	/*! 16位复数整型 */                         PDT_CInt16 = 8,
	/*! 32位复数整型 */                         PDT_CInt32 = 9,
	/*! 32位复数浮点型 */                       PDT_CFloat32 = 10,
	/*! 64位复数浮点型 */                       PDT_CFloat64 = 11,
	PDT_TypeCount = 12          /* maximum type # + 1 */
} RASDataType;

//获得数据类型的大小（字节数）
GEOMETRY_API int GEOMETRY_API_CALL RASGetDataTypeSize( RASDataType eDataType );


/*! 数据访问方式，只读或者可读写. */
typedef enum {
	/*! 只读 */							RA_ReadOnly = 0,
	/*! 可读写. */			            RA_Update = 1
} RASAccess;

/*! IO操作方式 */
typedef enum {
	/*! 读数据 */		RF_Read = 0,
	/*! 写数据 */		RF_Write = 1
} RASRWFlag;

/*! 颜色表 */
typedef struct ColorTable
{
	GByte c1;
	GByte c2;
	GByte c3;
	GByte c4;
} ColorTable;

//影像处理过程中进度的回调函数
typedef int (GEOMETRY_API_CALL *RASProgressFunc)(double dfComplete, const char *pszMessage, void *pProgressArg);


// 将数据拉伸到0到255
template <typename T>
void NormalLizeData(T* pSrcData,GByte* pBuffer,unsigned long lBufferSize,T MinValue,T MaxValue)
{
	double dbRange = static_cast<double>(MaxValue-MinValue);
	if (dbRange < 0.00001 && dbRange > -0.00001 )
	{
		if((MaxValue<=255)&&(MinValue>= 0))
		{
			#pragma omp parallel for
			for (GLonglong i = 0; i < lBufferSize; i ++ )
			{
				pBuffer[i] = pSrcData[i];
			}
		}
		else
		{
			#pragma omp parallel for
			for (GLonglong i = 0; i < lBufferSize; i ++ )
			{
				pBuffer[i] = 128;
			}
		}
	}
	else
	{
		#pragma omp parallel for
		for (GLonglong i = 0; i < lBufferSize; i ++ )
		{
			pBuffer[i] = (static_cast<float>(pSrcData[i])-MinValue) /dbRange  * 255.0;
			if(pBuffer[i]<0)
				pBuffer[i]=0;
			else if(pBuffer[i]>255)
				pBuffer[i]=255;
		}
	}
}

template <typename T>
void GetMinAndMax(T* pBuf,unsigned long lBufferSize,T LimtMin,T LimtMax,T& RealMin,T& RealMax)
{
	RealMin = LimtMax;
	RealMax = LimtMin;
	#pragma omp parallel for
	for (GLonglong k = 0; k < lBufferSize; k++)	
	{
		if (pBuf[k] > RealMax)
			RealMax = pBuf[k];
		if (pBuf[k] < RealMin)
			RealMin = pBuf[k];
	} 
}

#endif
