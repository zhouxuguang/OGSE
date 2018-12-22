/**
* @file              SysUtilFunc.h
* @brief             一些辅助函数，用于引擎内部
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月20日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSUTILFUNC_H_76F82CCF_3283_4567_8EEC_5745D04370C7__
#define __SYSUTILFUNC_H_76F82CCF_3283_4567_8EEC_5745D04370C7__

#include <vector>
#include <GL/glew.h>

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysMap;
	class SysMapLayer;
	class SysEnvelope;

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
	void MapExtentToLayerExtent(SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut);

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
	void LayerExtentToMapExtent(SysMap* pMap,SysMapLayer* pLayer,SysEnvelope& envOut);

	double* GetPloygonPoints(GeoPolygon* pPolygon,int& nPointCountOut,std::vector<GLsizei> &vecCounts,
		std::vector<std::vector<GLuint> >& vecIndices);

	void DrawGeometryHighLight(GeoGeometry* pGeometry);

	/**
	* @brief 根据字符串进行文字分割
	* @param[in] const char* pszFieldValue
	* @param[in] std::vector<std::string> &vecAllFieldValue
	* @return void
	* @author 周旭光
	* @date 2015年10月20日
	* @note 
	*/
	void SplitLabelStr(const char* pszFieldValue,std::vector<std::string> &vecAllFieldValue);
}

#endif // end of __SYSUTILFUNC_H_76F82CCF_3283_4567_8EEC_5745D04370C7__
