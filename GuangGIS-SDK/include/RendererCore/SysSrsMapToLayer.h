/**
* @file              SysSrsMapToLayer.h
* @brief             地图的SRS和图层的SRS之间相互转换
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年11月16日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSRSMAPTOLAYER_H_318B24D5_439B_45CB_A341_71138322AB32__
#define __SYSSRSMAPTOLAYER_H_318B24D5_439B_45CB_A341_71138322AB32__

#include "SysDrawCommon.h"

namespace SysSRS
{
	class CCoordTransformation;
}

namespace SysDraw
{
	class SysMap;
	class SysMapLayer;
	class SysEnvelope;

	class SYSDRAW_API SysSrsMapToLayer
	{
	public:
		/**
		* @brief 地图的矩形转换为图层的矩形
		* @param[in] SysMap* pMap
		* @param[in] SysMapLayer* pLayer
		* @param[out] SysEnvelope& envOut
		* @return void
		* @author 周旭光
		* @date 2015年8月20日
		* @note 
		*/
		static void MapToLayerExtent(SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut);

		/**
		* @brief 地图的矩形转换为图层的矩形
		* @param[in] SysMap* pMap
		* @param[in] const SysEnvelope& envIn
		* @param[in] SysMapLayer* pLayer
		* @param[in] SysEnvelope& envOut
		* @return void
		* @author 周旭光
		* @date 2015年11月16日
		* @note 
		*/
		static void MapToLayerExtent(SysMap* pMap,const SysEnvelope& envIn,SysMapLayer* pLayer,SysEnvelope& envOut);

		/**
		* @brief 图层的矩形转换为地图的矩形
		* @param[in] SysMap* pMap
		* @param[in] SysMapLayer* pLayer
		* @param[out] SysEnvelope& envOut
		* @return void
		* @author 周旭光
		* @date 2015年8月20日
		* @note 
		*/
		static void LayerToMapExtent(SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut);

		/**
		* @brief 图层的矩形转换为地图的矩形
		* @param[in] SysMap* pMap
		* @param[in] const SysEnvelope& envIn
		* @param[in] SysMapLayer* pLayer
		* @param[in] SysEnvelope& envOut
		* @return void
		* @author 周旭光
		* @date 2015年11月16日
		* @note 
		*/
		static void LayerToMapExtent(SysMap* pMap,const SysEnvelope& envIn, SysMapLayer* pLayer,SysEnvelope& envOut);

		static void TransExtent(SysSRS::CCoordTransformation* pTran,SysEnvelope& envOut);
	};

}


#endif // end of __SYSSRSMAPTOLAYER_H_318B24D5_439B_45CB_A341_71138322AB32__
