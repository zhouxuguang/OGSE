/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoVectorLayer.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��12��23��
@ ����ժҪ	: GDALդ������ͼ����
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOGDALIMAGELAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEOGDALIMAGELAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include "GeoImageLayer.h"

class GEOMETRY_API GeoGdalImageLayer : public GeoImageLayer
{
public:
	GeoGdalImageLayer(void);
	~GeoGdalImageLayer(void);

	int Open(const char* pszName,int bUpdata,void* pVoid = NULL);

	void Close();

	bool GetExtent(GeoEnvelope* pExtent) const;

	int GetWidth() const ;

	int GetHeight() const;

	int GetBandCount() const ;

	int HasColorTable() const ;

	int GetColorTable(int nBandIndex,std::vector<ColorTable> &vecColorTables) const ;

	GeoSpatialReference *GetSpatialRef() const ;

	void SetSpatialReference(GeoSpatialReference *pSRS);

	//��ȡһ�����ε�����
	int ReadData(int nBandIndex,
		int nXstart,
		int nYstart,
		int nWidth,
		int nHeight,
		int nBufXSize,
		int nBufYSize,
		RASDataType eDataType,
		void* poData);

	//��ȡָ�����ε�����
	int ReadData(int nBandCount,
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
		int 	nBandSpace = 0);

	//��һ����ȡĳһ�����ε�����
	int ReadDataNormalLized(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer);

	//��׼�������ȡĳһ�����ε�����
	int ReadDataStdDev(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer);

	//��ȡĳһ�������ݣ���ҪԤ��֪��������������
	int ReadDataBand(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		void* pBuffer) ;

	//�������굽��������ת��
	int PixelToWorld(long nCol,long nRow,double & dbX,double &dbY) const;

	//�����������������ת��
	int WorldToPixel(double dbX,double dbY,long &nCol,long &nRow) const;

	//����ĳһ���ε�ֱ��ͼ
	int GetHistogram( int nBand, int *panHistogram );

	bool GetPyramidSize( int nLevel,double& dbPyramidWdith,double& dbPyramidHeight)  const;

	int GetPyramidLevelCount() const;

	//���ĳһ���ε�������Сֵ
	bool GetBandMaxAndMin( int nBandIndex,double& dbMax,double& dbMin ) const;

	//���ĳһ���ε�ͳ��ֵ
	virtual bool GetStatistics(int nBandIndex,
		double *pdfMin,
		double *pdfMax,
		double *pdfMean,
		double *pdfStdDev,
		RASProgressFunc pfnProgress,
		void *pProgressData) const;

	bool GetBandStatisInfoEx(
		int nChannelIndex, 
		float* pfHist, 
		float& fMax, 
		float& fMin, 
		float& fMean, 
		float& fVar, 
		float fExclude) const;

private:
	int BuildOverviews	(
		int 	nListBands,
		int * 	panBandList,
		RASProgressFunc 	pfnProgress,
		void * 	pProgressData	 
		);

private:
	void* m_poDataset;		//GDAL���ݼ�
};

#endif // end of __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
