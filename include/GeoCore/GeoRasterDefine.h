/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoRasterDefine.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��12��23��
@ ����ժҪ	: GDALդ�����ݵĻ�������
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/

#ifndef __GEORASTERDEFINE_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEORASTERDEFINE_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include "GeoDefine.h"

//���ص���������
typedef enum 
{
	/*! δ֪���� */ 							PDT_Unknown = 0,
	/*! 8λ�޷����� */							PDT_Byte = 1,
	/*! 16λ�޷����� */							PDT_UInt16 = 2,
	/*! 16λ�з����� */						    PDT_Int16 = 3,
	/*! 32λ�޷������� */						PDT_UInt32 = 4,
	/*! 32λ�з������� */						PDT_Int32 = 5,
	/*! 32λ������ */							PDT_Float32 = 6,
	/*! 64λ������ */							PDT_Float64 = 7,
	/*! 16λ�������� */                         PDT_CInt16 = 8,
	/*! 32λ�������� */                         PDT_CInt32 = 9,
	/*! 32λ���������� */                       PDT_CFloat32 = 10,
	/*! 64λ���������� */                       PDT_CFloat64 = 11,
	PDT_TypeCount = 12          /* maximum type # + 1 */
} RASDataType;

//����������͵Ĵ�С���ֽ�����
GEOMETRY_API int GEOMETRY_API_CALL RASGetDataTypeSize( RASDataType eDataType );


/*! ���ݷ��ʷ�ʽ��ֻ�����߿ɶ�д. */
typedef enum {
	/*! ֻ�� */							RA_ReadOnly = 0,
	/*! �ɶ�д. */			            RA_Update = 1
} RASAccess;

/*! IO������ʽ */
typedef enum {
	/*! ������ */		RF_Read = 0,
	/*! д���� */		RF_Write = 1
} RASRWFlag;

/*! ��ɫ�� */
typedef struct ColorTable
{
	GByte c1;
	GByte c2;
	GByte c3;
	GByte c4;
} ColorTable;

//Ӱ��������н��ȵĻص�����
typedef int (GEOMETRY_API_CALL *RASProgressFunc)(double dfComplete, const char *pszMessage, void *pProgressArg);


// ���������쵽0��255
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
