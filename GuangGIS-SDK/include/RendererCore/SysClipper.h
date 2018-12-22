/**
* @file              SysClipper.h
* @brief             裁剪器，主要用于窗口裁剪
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年10月19日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSCLIPPER_H_88DD2E77_D966_497A_8091_363AECAEED31__
#define __SYSCLIPPER_H_88DD2E77_D966_497A_8091_363AECAEED31__

#include "SysDrawCommon.h"

namespace SysDraw
{
	class SysEnvelope;
	class SysGeometry;
	class SysPolygon;
	class SysCoordinate;

	class SYSDRAW_API SysClipper
	{
	public:
		SysClipper(void);

		~SysClipper(void);

		/**
		* @brief 获得一个坐标串上指定距离的坐标点，即插值
		* @param[in] ysCoordinate * poPoints
		* @param[in] int nPointCount
		* @param[in] double dfDistance
		* @param[in] SysCoordinate * pPoint
		* @param[in] double *pAngle
		* @return void
		* @author 周旭光
		* @date 2015年10月22日
		* @note 
		*/
		static void GetValuePoint( 
								SysCoordinate * poPoints, 
								int nPointCount, 
								double dfDistance, 
								SysCoordinate * pPoint ,
								double *pAngle);

		static double GetLineStringLength(SysCoordinate * poPoints, int nPointCount);

		/**
		* @brief 折线裁剪
		* @param[in] ysCoordinate* poPoints
		* @param[in] int nCount
		* @param[in] SysEnvelope& rect
		* @param[out] std::vector<int> &vecIndex
		* @param[out] std::vector<SysCoordinate> &outPoints
		* @return bool
		* @author 周旭光
		* @date 2015年10月19日
		* @note 
		*/
		static bool PolylineClip(
								SysCoordinate* poPoints,
								int nCount,SysEnvelope& rect,
								std::vector<int> &vecIndex,
								std::vector<SysCoordinate> &outPoints);

		/**
		* @brief 折线裁剪
		* @param[in] ouble* poCoords xyxy排列
		* @param[in] int nCount
		* @param[in] SysEnvelope& rect
		* @param[out] std::vector<int> &vecIndex
		* @param[out] std::vector<double> &outPoints
		* @return bool
		* @author 周旭光
		* @date 2015年10月19日
		* @note 
		*/
		static bool PolylineClipEx(
								double* poCoords,
								int nCount,SysEnvelope& rect,
								std::vector<int> &vecIndex,
								std::vector<double> &outPoints);

		/**
		* @brief 多边形裁剪
		* @param[in] SysPolygon* pPolygon
		* @param[in] SysEnvelope& rect
		* @return SysGeometry*
		* @author 周旭光
		* @date 2015年10月22日
		* @note 
		*/
		static SysGeometry* PolygonClip(SysPolygon* pPolygon,SysEnvelope& rect);

	};

}

#endif // end of __SYSCLIPPER_H_88DD2E77_D966_497A_8091_363AECAEED31__
