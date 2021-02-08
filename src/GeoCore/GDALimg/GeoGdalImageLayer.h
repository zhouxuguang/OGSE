/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoVectorLayer.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月23日
@ 内容摘要	: GDAL栅格数据图层类
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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

	//读取一个波段的数据
	int ReadData(int nBandIndex,
		int nXstart,
		int nYstart,
		int nWidth,
		int nHeight,
		int nBufXSize,
		int nBufYSize,
		RASDataType eDataType,
		void* poData);

	//读取指定波段的数据
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

	//归一化读取某一个波段的数据
	int ReadDataNormalLized(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer);

	//标准差拉伸读取某一个波段的数据
	int ReadDataStdDev(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer);

	//读取某一波段数据，需要预先知道像素数据类型
	int ReadDataBand(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		void* pBuffer) ;

	//像素坐标到世界坐标转换
	int PixelToWorld(long nCol,long nRow,double & dbX,double &dbY) const;

	//世界坐标打到像素坐标转换
	int WorldToPixel(double dbX,double dbY,long &nCol,long &nRow) const;

	//计算某一波段的直方图
	int GetHistogram( int nBand, int *panHistogram );

	bool GetPyramidSize( int nLevel,double& dbPyramidWdith,double& dbPyramidHeight)  const;

	int GetPyramidLevelCount() const;

	//获得某一波段的最大和最小值
	bool GetBandMaxAndMin( int nBandIndex,double& dbMax,double& dbMin ) const;

	//获得某一波段的统计值
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
	void* m_poDataset;		//GDAL数据集
};

#endif // end of __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
