/**
* @file              SysDisplayTransform.h
* @brief             地理坐标和设备坐标之间的转换
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月26日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDISPLAYTRANSFORM_H_030BA0AA_9D1B_4E49_9ABF_DEAB0333BAFD__
#define __SYSDISPLAYTRANSFORM_H_030BA0AA_9D1B_4E49_9ABF_DEAB0333BAFD__

#include "SysDrawCommon.h"
#include "SysDrawDef.h"
#include "SysEnvelope.h"


namespace SysDraw
{

	class SYSDRAW_API SysDisplayTransform
	{
	public:
		/**
		* @brief SysDisplayTransform
		* @return 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysDisplayTransform();

		/**
		* @brief SysDisplayTransform
		* @param[in] double dbMinx	//可见地理范围最小x
		* @param[in] double dbMaxx
		* @param[in] double dbMiny
		* @param[in] double dbMaxy
		* @param[in] double dbMapUnitsPerPixel	//每个像素代表的实际地理长度
		* @return 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysDisplayTransform(
			double dbMinx,
			double dbMaxx,
			double dbMiny,
			double dbMaxy,
			double dbMapUnitsPerPixel);

		/**
		* @brief SysDisplayTransform
		* @param[in] SysDraw::SysEnvelope& VisualBounds 可见地理范围
		* @param[in] double dbMapUnitsPerPixel 每个像素代表的实际地理长度
		* @return 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		SysDisplayTransform(SysDraw::SysEnvelope& VisualBounds,double dbMapUnitsPerPixel);

		SysDisplayTransform(const SysDisplayTransform& OtherTran);

		SysDisplayTransform& operator=(const SysDisplayTransform& OtherTran);

		~SysDisplayTransform(void);

		void GetBounds(SysDraw::SysEnvelope& Bounds) const;

		void SetBounds(const SysDraw::SysEnvelope& Bounds);

		void GetVisibleBounds(SysDraw::SysEnvelope& Bounds) const;

		void SetVisibleBounds(SysDraw::SysEnvelope& Bounds);

		void SetVisibleBoundsExt(const SysDraw::SysEnvelope& Bounds);

		void GetExportFrame(RECT& rtFrame) const;

		void SetExportFrame(const RECT& rtFrame);

		void GetExportFrameSize(double& width,double& height) const;

		void SetExportFrameSize(double width,double height);

		void SetViewRect(const RECT& viewRect);

		/**
		* @brief SetMapUnitsPerPixel
		* @param[in] double dbMapUnitsPerPixel
		* @return void
		* @author 周旭光
		* @date 2015年7月16日
		* @note 
		*/
		void SetMapUnitsPerPixel(double dbMapUnitsPerPixel);

		/**
		* @brief GetMapUnitsPerPixel
		* @return double
		* @author 周旭光
		* @date 2015年7月16日
		* @note 
		*/
		double GetMapUnitsPerPixel() const;

		/**
		* @brief  设备坐标转换为地理坐标
		* @param[in] long lX
		* @param[in] long lY
		* @param[out] double& x
		* @param[out] double& y
		* @return void 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void  ToMapPoint(long lX, long lY, double& x, double& y) const;

		/**
		* @brief 设备坐标转换为地理坐标
		* @param[in] const POINT& screenPoint 屏幕坐标
		* @param[out] SysCoordinate &geoPoint 转换后的地理坐标
		* @return void 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void  ToMapPoint(const POINT& screenPoint, SysCoordinate &geoPoint) const;

		/**
		* @brief 屏幕范围转换为地理范围
		* @param[in] const RECT& viewRect
		* @param[out] SysEnvelope& envGeo
		* @return void 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		void  ToMapRect(const RECT& viewRect, SysEnvelope& envGeo) const;

		/**
		* @brief  地理坐标转换为设备坐标
		* @param[in] double dbGeoX
		* @param[in] double dbGeoY
		* @param[out] long& lX
		* @param[out] long& lY
		* @return long 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool ToScreenPoint(double dbGeoX, double dbGeoY,long& lX, long& lY) const;

		/**
		* @brief 地理坐标转换为设备坐标
		* @param[in] const SysCoordinate& geoPoint 地理坐标
		* @param[out] POINT& screenPoint 转换后的屏幕坐标
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool ToScreenPoint(const SysCoordinate& geoPoint,POINT& screenPoint) const;

		/**
		* @brief 地理范围转换为屏幕范围
		* @param[in] const SysEnvelope &envGeo
		* @param[in] RECT &rectView
		* @return long 
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool ToScreenRect(const SysEnvelope &envGeo,RECT &rectView) const;

		/**
		* @brief 地理范围转换为屏幕范围
		* @param[in] const SysEnvelope &envGeo
		* @param[in] RECTD &rectView
		* @return bool
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		bool ToScreenRect(const SysEnvelope &envGeo,RECTD &rectView) const;

		/**
		* @brief 获得视口的宽度
		* @return long
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		long GetViewWidth() const;

		/**
		* @brief 获得视口的高度
		* @return long
		* @author 周旭光
		* @date 2015年6月26日
		* @note 
		*/
		long GetViewHeight() const;

	private:
		SysDraw::SysEnvelope  m_Bounds;						//全图地理坐标范围
		SysDraw::SysEnvelope  m_VisibleBounds;				//“可见”地理坐标范围

		RECT	  m_ViewRect;						//视口的范围，屏幕画图的区域
		RECT	  m_DeviceFrame;					//“可见”地理范围的设备坐标范围
		double	  m_dbDegree;						//旋转角度
		double	  m_dbMapUnitsPerPixel;				//每个像素代表的实际地理长度
		double	  m_dbPixelsPerMapUnit;				//每个单位地理距离代表的像素个数
		double    m_dbDeviceWidth;
		double    m_dbDeviceHeight;

		long		m_lViewWidth;					//视口的宽度
		long		m_lViewHeight;					//视口的高度

		void CalculateVisibleExtents();
	};

}

#endif // end of __SYSDISPLAYTRANSFORM_H_030BA0AA_9D1B_4E49_9ABF_DEAB0333BAFD__
