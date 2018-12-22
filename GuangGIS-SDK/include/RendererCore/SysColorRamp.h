/**
* @file              SysColorRamp.h
* @brief             颜色条（主要实现颜色的渐变算法）
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSCOLORRAMP_H_71A8A3B6_79FB_41CB_A7C1_18437FFCFAF8__
#define __SYSCOLORRAMP_H_71A8A3B6_79FB_41CB_A7C1_18437FFCFAF8__

#include "SysDrawCommon.h"
#include "SysDrawDef.h"

namespace SysDraw
{
	class SysRGBAColor;

	class SYSDRAW_API SysColorRamp
	{
	public:
		/**
		* @brief SysColorRamp
		* @param[in] const SysRGBAColor& startColor 开始的颜色
		* @param[in] const SysRGBAColor& endColor 结束的颜色
		* @return 
		* @author 周旭光
		* @date 2015年8月27日
		* @note 
		*/
		SysColorRamp(const SysRGBAColor& startColor,const SysRGBAColor& endColor);

		/**
		* @brief SysColorRamp
		* @param[in] const SysColorItem& startColor 开始的颜色
		* @param[in] const SysColorItem& endColor 结束的颜色
		* @return 
		* @author 周旭光
		* @date 2015年8月27日
		* @note 
		*/
		SysColorRamp(const SysColorItem& startColor,const SysColorItem& endColor);

		~SysColorRamp(void);

		/**
		* @brief 根据颜色条获得渐变的颜色值序列
		* @param[in] SysRGBAColor* pColors
		* @param[in] int nCount
		* @return void
		* @author 周旭光
		* @date 2015年8月27日
		* @note 
		*/
		void GetColors(SysRGBAColor* pColors,int nCount) const;

		/**
		* @brief 根据颜色条获得渐变的颜色值序列
		* @param[in] SysColorItem* pColors
		* @param[in] int nCount
		* @return void
		* @author 周旭光
		* @date 2015年8月27日
		* @note 
		*/
		void GetColors(SysColorItem* pColors,int nCount) const;

	private:
		SysRGBAColor* m_pStartColor;			//开始的颜色
		SysRGBAColor* m_pEndColor;				//结束的颜色
	};

}


#endif // end of __SYSCOLORRAMP_H_71A8A3B6_79FB_41CB_A7C1_18437FFCFAF8__