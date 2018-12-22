/**
* @file              SysDrawCanvas.h
* @brief             地图绘图画布
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月10日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDRAWCANVAS_H_9AA51398_0D4D_4776_B255_78D1D83A0FF8__
#define __SYSDRAWCANVAS_H_9AA51398_0D4D_4776_B255_78D1D83A0FF8__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysMap;
	class SysMapLayer;
	class SysEnvelope;
	class SysRenderContext;

	class SYSDRAW_API SysDrawCanvas
	{
	public:
		explicit SysDrawCanvas(SysRenderContext* pRenderContext);
		~SysDrawCanvas(void);

		/**
		* @brief 窗口大小改变函数
		* @param[in] eUInt32 nType
		* @param[in] int cx 宽度
		* @param[in] int cy	高度
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void OnSize(eUInt32 nType, int cx, int cy);

		/**
		* @brief 设置当前的渲染环境
		* @param[in] SysRenderContext* pRenderContext 当前的渲染环境
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void SetRenderContext(SysRenderContext* pRenderContext);

		/**
		* @brief 绘制地图
		* @param[in] SysMap* pMap 当前地图
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void DrawMap(SysMap* pMap) const;

		/**
		* @brief 绘制地图,局部范围
		* @param[in] SysMap* pMap
		* @param[in] const SysEnvelope& envDraw 绘制的范围
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void DrawMap(SysMap* pMap,const SysEnvelope& envDraw) const;

		/**
		* @brief 绘制图层
		* @param[in] SysMap* pMap 当前地图
		* @param[in] SysMapLayer* pLayer 当前图层
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void DrawLayer(SysMap* pMap,SysMapLayer* pLayer) const;

		/**
		* @brief 绘制图层,局部范围
		* @param[in] SysMap* pMap
		* @param[in] SysMapLayer* pLayer
		* @param[in] const SysEnvelope& envDraw 绘制的范围
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void DrawLayer(SysMap* pMap,SysMapLayer* pLayer,const SysEnvelope& envDraw) const;

		/**
		* @brief 
		* @param[in] SysMap* pMap
		* @return void
		* @author 周旭光
		* @date 2015年9月17日
		* @note 
		*/
		void DrawGraphicLayers(SysMap* pMap) const;

		/**
		* @brief 绘制地图浏览标绘图层
		* @param[in] SysMap* pMap
		* @return void
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		void DrawBrowseLayer(SysMap* pMap) const;

		/**
		* @brief 绘制标绘层
		* @param[in] SysMap* pMap
		* @param[in] bool bDrawAll
		* @param[in] int nIndex
		* @return void
		* @author 周旭光
		* @date 2015年8月13日
		* @note 
		*/
		void DrawMarkLayer(SysMap* pMap,bool bDrawAll = true, int nIndex = -1) const;

		/**
		* @brief 获得客户区宽度
		* @return int
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		int GetViewWidth() const;

		/**
		* @brief 获得客户区高度
		* @return int
		* @author 周旭光
		* @date 2015年7月10日
		* @note 
		*/
		int GetViewHeight() const;

	private:
		SysRenderContext *m_pRenderContext;
		int m_nViewWidth;					//客户区宽度
		int m_nViewHeight;					//客户区高度
	};

}

#endif // end of __SYSDRAWCANVAS_H_9AA51398_0D4D_4776_B255_78D1D83A0FF8__
