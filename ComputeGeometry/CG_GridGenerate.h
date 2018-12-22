/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2013
* 文件名称	: CG_GridGenerate.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月21日
* 内容摘要	: 空间插值算法，即数据的网格化
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/

#ifndef __CG_GRIDGENERATE_H_60C8CA6D_A89B_417E_9E1F_9997AC3E50BA__
#define __CG_GRIDGENERATE_H_60C8CA6D_A89B_417E_9E1F_9997AC3E50BA__

#include "CG_Common.h"

#ifdef  __cplusplus
extern "C" {
#endif

/** 插值算法 */
typedef enum {
   GGA_InverseDistanceToAPower = 1,
   GGA_MovingAverage = 2,
   GGA_MovingFitting = 3,
   GGA_NearestNeighbor = 4,
   GGA_MetricMinimum = 5,
   GGA_MetricMaximum = 6,
   GGA_MetricRange = 7,
   GGA_MetricCount = 8,
   GGA_MetricAverageDistance = 9,
   GGA_MetricAverageDistancePts = 10
} GridAlgorithm;

/** 反比距离权重算法参数 */
typedef struct
{
    double  dbPower;		/*! 权重幂方. */
    double  dbSmoothing;	 /*! 平滑参数. */
    double  dbRadius1;
    double  dbRadius2;
    double  dbAngle;
    unsigned int nMaxPoints;
    unsigned int nMinPoints;
    double  dbNoDataValue;	 /*! 无效值. */
} GridInverseDistanceToAPowerOptions;

/** 移动平均法参数信息 */
typedef struct
{
    double  dbRadius1;		//X方向的搜索半径
    double  dbRadius2;		//Y方向的搜索半径
    double  dbAngle;		//椭圆旋转角度,顺时针,十进制度
    unsigned int nMinPoints;	//最少点的个数
    double  dbNoDataValue;		//无效值
} GridMovingAverageOptions;

/** 移动曲面拟合法参数信息 */
typedef struct
{
	double  dbRadius;		//搜索半径
	double  dbNoDataValue;		//无效值
} GridMovingFittingOptions;

/** Nearest neighbor method control options */
typedef struct
{
    /*! The first radius (X axis if rotation angle is 0) of search ellipse. */
    double  dbRadius1;
    /*! The second radius (Y axis if rotation angle is 0) of search ellipse. */
    double  dbRadius2;
    /*! Angle of ellipse rotation in degrees.
     *
     * Ellipse rotated counter clockwise.
     */
    double  dbAngle;
    /*! No data marker to fill empty points. */
    double  dbNoDataValue;
} GridNearestNeighborOptions;



 COMPUTEGEOMETRY_API int GridInterpolation( GridAlgorithm eAlgorithmType, const void *poOptions,
			   unsigned int nPointCount,
			   const double *padfX, const double *padfY, const double *padfZ,
			   double dbXMin, double dbXMax, double dbYMin, double dbYMax,
			   unsigned int nXSize, unsigned int nYSize, float *pData,
			   /*GDALProgressFunc pfnProgress,*/ void *pProgressArg );

 COMPUTEGEOMETRY_API int  GridInverseDistanceToAPowerNoSearch( const void *poOptions, unsigned int nPoints,
										const double *padfX, const double *padfY,
										const double *padfZ,
										double dfXPoint, double dfYPoint,
										double *pdfValue,
										void* hExtraParamsIn ) ;

 COMPUTEGEOMETRY_API int GridMovingAverage( const void *poOptions, unsigned int nPoints,
	 const double *padfX, const double *padfY,
	 const double *padfZ,
	 double dfXPoint, double dfYPoint, double *pdfValue,
	 void* hExtraParamsIn );

 COMPUTEGEOMETRY_API int GridMovingFitting( const void *poOptions, unsigned int nPoints,
	 const double *padfX, const double *padfY,
	 const double *padfZ,
	 double dfXPoint, double dfYPoint, double *pdfValue,
	 void* hExtraParamsIn );

 COMPUTEGEOMETRY_API int GridTrendSurface( const void *poOptions, unsigned int nPoints,
	 const double *padfX, const double *padfY,
	 const double *padfZ,
	 double dfXPoint, double dfYPoint, double *pdfValue,
	 void* hExtraParamsIn );


#ifdef  __cplusplus
}
#endif

#endif
