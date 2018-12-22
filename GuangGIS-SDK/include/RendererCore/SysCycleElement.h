/**
* @file              SysCycleElement.h
* @brief             圆图元对象
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年9月17日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSCYCLEELEMENT_H_2370AD9F_30EC_429F_90C7_431C2BE219EE__
#define __SYSCYCLEELEMENT_H_2370AD9F_30EC_429F_90C7_431C2BE219EE__

#include "SysGraphElement.h"

namespace SysDraw
{

	class SysCoordinate;

	class SYSDRAW_API SysCycleElement : public SysGraphElement
	{
	public:
		SysCycleElement(void);

		~SysCycleElement(void);

		virtual bool Move(double dx, double dy);

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
		* @brief 点是否在圆内
		* @param[in] double x
		* @param[in] double y
		* @return bool
		* @author 周旭光
		* @date 2015年8月3日
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
		* @brief 获取周长
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
		* @brief 设置中心点坐标
		* @param[in] const double dbX
		* @param[in] const double dbY
		* @return void
		* @author 周旭光
		* @date 2015年9月17日
		* @note 
		*/
		void SetCenterPoint(const double dbX,const double dbY);

		/**
		* @brief 设置圆的半径
		* @param[in] const double dbRadius
		* @return void
		* @author 周旭光
		* @date 2015年9月17日
		* @note 
		*/
		void SetRadius(const double dbRadius);

	private:
		SysCoordinate *m_pCenterPoint;			//圆的圆心
		double m_dbRadius;						//半径
	};

}

#endif // end of __SYSCYCLEELEMENT_H_2370AD9F_30EC_429F_90C7_431C2BE219EE__
