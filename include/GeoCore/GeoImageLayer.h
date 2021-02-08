/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoVectorLayer.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��12��23��
@ ����ժҪ	: դ������ͼ����,��Ҫ����Ӱ�����ݵ�IO
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOIMAGELAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEOIMAGELAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include <vector>

#include "GeoLayer.h"
#include "GeoRasterDefine.h"


class GEOMETRY_API GeoImageLayer : public GeoLayer
{
public:
	GeoImageLayer(void);
	virtual ~GeoImageLayer(void);

	//���úͻ����С�������
	virtual bool GetExtent(GeoEnvelope* pExtent) const = 0;

	virtual int Open(const char* pszName,int bUpdata,void* pVoid = NULL) = 0;

	virtual void Close() = 0;

	virtual int GetWidth() const = 0;

	virtual int GetHeight() const = 0;

	virtual int GetBandCount() const = 0;

	virtual int HasColorTable() const = 0;

	virtual int GetColorTable(int nBandIndex,std::vector<ColorTable> &vecColorTables) const = 0;

	virtual GeoSpatialReference *GetSpatialRef() const { return NULL; }

	virtual void SetSpatialReference(GeoSpatialReference *pSRS) = 0;

	//��ȡһ�����ε�����
	virtual int ReadData(int nBandIndex,
		int nXstart,
		int nYstart,
		int nWidth,
		int nHeight,
		int nBufXSize,
		int nBufYSize,
		RASDataType eDataType,
		void* poData) = 0;

	//��ȡָ�����ε�����
	virtual int ReadData(int nBandCount,
		int		*pBandIndex,
		int		nXstart,
		int		nYstart,
		int		nWidth,
		int		nHeight,
		int		nBufXSize,
		int		nBufYSize,
		RASDataType eDataType,
		void    *poData,
		int 	nPixelSpace = 0,
		int 	nLineSpace = 0,
		int 	nBandSpace = 0) = 0;

	//��һ����ȡĳһ�����ε�����
	virtual int ReadDataNormalLized(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer) = 0;

	//��׼�������ȡĳһ�����ε�����
	virtual int ReadDataStdDev(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer) = 0;

	//��ȡĳһ�������ݣ���ҪԤ��֪��������������
	virtual int ReadDataBand(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		void* pBuffer) = 0;

	virtual int PixelToWorld(long nCol,long nRow,double & dbX,double &dbY) const = 0;

	virtual int WorldToPixel(double dbX,double dbY,long &nCol,long &nRow) const = 0;

	//����ĳһ���ε�ֱ��ͼ
	virtual int GetHistogram( int nBand, int *panHistogram ) = 0;

	//���������ĳһ���ϵ����س��ȺͿ��
	virtual bool GetPyramidSize( int nLevel,double& dbPyramidWdith,double& dbPyramidHeight) const = 0;

	//��ý������Ĳ���(������ԭʼ��)
	virtual int GetPyramidLevelCount() const = 0;

	//���ĳһ���ε�������Сֵ,nBandIndex��0��ʼ
	virtual bool GetBandMaxAndMin( int nBandIndex,double& dbMax,double& dbMin ) const = 0;

	//���ĳһ���ε�ͳ��ֵ
	virtual bool GetStatistics(int nBandIndex,
		double *pdfMin,
		double *pdfMax,
		double *pdfMean,
		double *pdfStdDev,
		RASProgressFunc 	pfnProgress,
		void * 	pProgressData) const = 0;

	//����ͳ�ƣ��ٶȻغܿ�
	virtual bool GetBandStatisInfoEx(
		int nChannelIndex, 
		float* pfHist, 
		float& fMax, 
		float& fMin, 
		float& fMean, 
		float& fVar, 
		float fExclude) const = 0;


private:
	virtual int BuildOverviews	(
		int 	nListBands,
		int * 	panBandList,
		RASProgressFunc 	pfnProgress,
		void * 	pProgressData	 
		) = 0;

protected:
	int m_nWidth;		//Ӱ��ͼ��Ŀ��
	int m_nHeight;		//Ӱ��ͼ��ĸ߶�

	int m_nBandCount;	//Ӱ��Ĳ�����Ŀ
};

GEOMETRY_API GeoImageLayer* GeoImageOpen(const char* pszName,int bUpdata,void* pVoid = NULL);

#endif // 
