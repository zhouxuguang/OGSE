/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoVectorLayer.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月23日
@ 内容摘要	: 栅格数据图层类,主要用于影像数据的IO
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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

	//设置和获得最小外包矩形
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

	//读取一个波段的数据
	virtual int ReadData(int nBandIndex,
		int nXstart,
		int nYstart,
		int nWidth,
		int nHeight,
		int nBufXSize,
		int nBufYSize,
		RASDataType eDataType,
		void* poData) = 0;

	//读取指定波段的数据
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

	//归一化读取某一个波段的数据
	virtual int ReadDataNormalLized(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer) = 0;

	//标准差拉伸读取某一个波段的数据
	virtual int ReadDataStdDev(int nBandIndex,
		int nLevel,
		int nXstart,
		int nYstart, 
		int nWidth, 
		int nHeight, 
		int nBuffSizeX, 
		int nBuffSizeY, 
		GByte* pBuffer) = 0;

	//读取某一波段数据，需要预先知道像素数据类型
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

	//计算某一波段的直方图
	virtual int GetHistogram( int nBand, int *panHistogram ) = 0;

	//计算金字塔某一层上的像素长度和宽度
	virtual bool GetPyramidSize( int nLevel,double& dbPyramidWdith,double& dbPyramidHeight) const = 0;

	//获得金字塔的层数(不包括原始层)
	virtual int GetPyramidLevelCount() const = 0;

	//获得某一波段的最大和最小值,nBandIndex从0开始
	virtual bool GetBandMaxAndMin( int nBandIndex,double& dbMax,double& dbMin ) const = 0;

	//获得某一波段的统计值
	virtual bool GetStatistics(int nBandIndex,
		double *pdfMin,
		double *pdfMax,
		double *pdfMean,
		double *pdfStdDev,
		RASProgressFunc 	pfnProgress,
		void * 	pProgressData) const = 0;

	//粗略统计，速度回很快
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
	int m_nWidth;		//影像图层的宽度
	int m_nHeight;		//影像图层的高度

	int m_nBandCount;	//影像的波段数目
};

GEOMETRY_API GeoImageLayer* GeoImageOpen(const char* pszName,int bUpdata,void* pVoid = NULL);

#endif // 
