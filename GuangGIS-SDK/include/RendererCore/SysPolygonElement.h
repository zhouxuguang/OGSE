/**
* @file              SysPolygonElement.h
* @brief             多边形图元定义
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月11日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSPOLYGONELEMENT_H_C4A49E91_9D0E_4A13_91C5_99C872CE595A__
#define __SYSPOLYGONELEMENT_H_C4A49E91_9D0E_4A13_91C5_99C872CE595A__

#include "SysGraphElement.h"

namespace SysDraw
{

	class SysLineElement;

	class SYSDRAW_API SysPolygonElement : public SysGraphElement
	{
	public:
		SysPolygonElement(void);

		~SysPolygonElement(void);

		/**
		* @brief 给多边形的外环添加节点
		* @param[in] double dx
		* @param[in] double dy
		* @return void
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		void AddPoint(double dx,double dy);

		/**
		* @brief 添加内岛
		* @param[in] SysLineElement* pLineRing
		* @return void
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		void AddIsLand(SysLineElement* pLineRing);

		/**
		* @brief 图元的移动(整体移动)
		* @param[in] double dx
		* @param[in] double dy
		* @return bool
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual bool Move(double dx, double dy);

		/**
		* @brief 移动顶点(只支持移动外环的节点)
		* @param[in] int nIndex
		* @param[in] double dx
		* @param[in] double dy
		* @return bool
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual bool MovePoint(int nIndex, double dx, double dy);

		/**
		* @brief 把外环的某个节点移动到某个坐标(只支持移动外环的节点)
		* @param[in] int nIndex
		* @param[in] double dbX
		* @param[in] double dbY
		* @return bool
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual bool MovePointTo(int nIndex, double dbX, double dbY);

		/**
		* @brief 获得最小外包矩形
		* @param[in] void
		* @return SysEnvelope&
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual SysEnvelope& GetEnvelope(void) const;

		/**
		* @brief PointInArea
		* @param[in] double x
		* @param[in] double y
		* @return bool
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		virtual bool PointInArea(double x,double y) const;

		/**
		* @brief 获取中心点
		* @param[out] double &x
		* @param[out] double &y
		* @return void
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual void GetCenterPoint(double &x,double &y) const;

		/**
		* @brief /获取周长()
		* @return double
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual double GetPerimeter() const;

		/**
		* @brief 获取面积(针对封闭图元)
		* @return double
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual double GetArea() const;

		/**
		* @brief 获得内岛的个数
		* @return int
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		int GetIsLandCount() const;

		/**
		* @brief 获得第几个内岛
		* @param[in] int nIndex
		* @return SysLineElement*
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		SysLineElement* GetIsLand(int nIndex) const;

		/**
		* @brief 获得多边形外环
		* @return SysLineElement*
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		SysLineElement* GetExteriorRing() const;

	private:
		SysLineElement* m_pExteriorRing;						//外环		
		std::vector<SysLineElement* > m_vecIsLands;			//内岛
	};

}

#endif // end of __SYSPOLYGONELEMENT_H_C4A49E91_9D0E_4A13_91C5_99C872CE595A__
