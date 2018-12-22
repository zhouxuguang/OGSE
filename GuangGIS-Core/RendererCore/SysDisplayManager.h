/**
* @file              SysDisplayManager.h
* @brief             显示管理类，主要负责地图浏览等操作
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年12月15日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDISPLAYMANAGER_H_02E5DADD_4E54_9DEB_FA46C715756E__
#define __SYSDISPLAYMANAGER_H_02E5DADD_4E54_9DEB_FA46C715756E__


namespace SysDraw
{

	class SysEnvelope;
	class SysMapLayer;

	class SysDisplayManager
	{
	public:
		SysDisplayManager(void);

		~SysDisplayManager(void);

		/**
		* @brief 移动地图
		* @param[in]  double dx
		* @param[in] double dy
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void MoveContent( double dx, double dy );

		/**
		* @brief 缩放地图
		* @param[in]  int delta
		* @param[in] double x
		* @param[in] double y
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomContent( int delta, double x, double y );

		/**
		* @brief 缩放到某个范围，如拉框放大,缩放到全图等操作
		* @param[in] const SysEnvelope& envNew
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomInContent(const SysEnvelope& envNew);

		/**
		* @brief 拉框缩小
		* @param[in] const SysEnvelope& envNew 当前视图的范围
		* @param[in] double x 当前拉框的中心点
		* @param[in] double y
		* @return void
		* @author 周旭光
		* @date 2015年8月5日
		* @note 
		*/
		void ZoomOutContent(const SysEnvelope& envNew,double x,double y);

		/**
		* @brief 缩放到图层
		* @param[in] const SysMapLayer* pMapLayer
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomToLayer(const SysMapLayer* pMapLayer);

		/**
		* @brief 缩放到当前栅格图层同样的分辨率，即1:1显示
		* @param[in] const SysMapLayer* pMapLayer
		* @return void
		* @author 周旭光
		* @date 2015年7月18日
		* @note 
		*/
		void ZoomActualSize(const SysMapLayer* pMapLayer);
	};

}

#endif
