/**
* @file              SysGraphElement.h
* @brief             图元的基类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月3日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSGRAPHELEMENT_H_6E918BE8_BEEF_496E_9ECA_3103A339E181__
#define __SYSGRAPHELEMENT_H_6E918BE8_BEEF_496E_9ECA_3103A339E181__

#include "SysDrawCommon.h"
#include "SysDrawDef.h"

namespace SysDraw
{
	class SysEnvelope;

	class SYSDRAW_API SysGraphElement
	{
	public:
		explicit SysGraphElement(GraphicsElementType eGraType);

		virtual ~SysGraphElement(void);

		/**
		* @brief Clone
		* @return SysGraphElement*
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual SysGraphElement* Clone() const;

		/**
		* @brief GetType
		* @return GraphicsElementType
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		GraphicsElementType GetElementType() const;

		/**
		* @brief 图元的移动(整体移动)
		* @param[in] double dx
		* @param[in] double dy
		* @return bool
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		virtual bool Move(double dx, double dy) = 0;

		/**
		* @brief 移动顶点
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
		* @brief MovePointTo
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
		virtual SysEnvelope& GetEnvelope(void) const = 0;

		/**
		* @brief SetAlpha
		* @param[in] int nAlpha
		* @return void
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		void SetAlpha(int nAlpha);

		/**
		* @brief GetAlpha
		* @return int
		* @author 周旭光
		* @date 2015年8月3日
		* @note 
		*/
		int GetAlpha() const;

		/**
		* @brief PointInArea
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

	private:
		GraphicsElementType m_GraphicType;				//图元的类型
		int m_nAlphaPrecent;							//透明度
	};

	SYSDRAW_API SysGraphElement* CreateGraphElement(GraphicsElementType eGraphicType);

	SYSDRAW_API void DestroyGraphElement(SysGraphElement* pGraphElem);

}

#endif // end of __SYSGRAPHELEMENT_H_6E918BE8_BEEF_496E_9ECA_3103A339E181__
