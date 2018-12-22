/**
* @file              SysRGBColor.h
* @brief             RGBA颜色封装类，适应多个平台
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月29日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSRGBCOLOR_H_196793B0_415A_45A4_94AD_E48A1AB62395__
#define __SYSRGBCOLOR_H_196793B0_415A_45A4_94AD_E48A1AB62395__

#include "SysDrawCommon.h"


namespace SysDraw
{


	class SYSDRAW_API SysRGBAColor
	{
	public:
		SysRGBAColor();

		/**
		* @brief 使用四个浮点数创建RGB颜色，各个分量必须在0-1之间，超过范围截断 
		* @param[in] float fRed
		* @param[in] float fGreen
		* @param[in] float fBlue
		* @param[in] float fAlpha
		* @return 
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		SysRGBAColor(float fRed,float fGreen,float fBlue,float fAlpha = 1.0);

		/**
		* @brief SysRGBAColor
		* @param[in] const SysRGBAColor& rhs
		* @return 
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		SysRGBAColor(const SysRGBAColor& rhs);

		/**
		* @brief operator = 
		* @param[in] const SysRGBAColor& rhs
		* @return SysRGBAColor& 
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		SysRGBAColor& operator = (const SysRGBAColor& rhs);

		/**
		* @brief operator + 
		* @param[in] const SysRGBAColor& rhs
		* @return SysRGBAColor& 
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		SysRGBAColor& operator + (const SysRGBAColor& rhs);

		/**
		* @brief operator * 
		* @param[in] const SysRGBAColor& rhs
		* @return SysRGBAColor& 
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		SysRGBAColor& operator * (const SysRGBAColor& rhs);

		~SysRGBAColor(void);

		/**
		* @brief 返回颜色中的红色部分，在0-1区间
		* @return float
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		float GetRed() const;

		/**
		* @brief 返回颜色中的绿色部分，在0-1区间
		* @return float
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		float GetGreen() const;

		/**
		* @brief 返回颜色中的蓝色部分，在0-1区间
		* @return float
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		float GetBlue() const;

		/**
		* @brief 返回颜色中的alpha部分，在0-1区间
		* @return float
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		float GetAlpha() const;

		/**
		* @brief 使用四个浮点数设置RGB颜色，各个分量必须在0-1之间，超过范围截断
		* @param[in] float fRed
		* @param[in] float fGreen
		* @param[in] float fBlue
		* @param[in] float fAlpha
		* @return void
		* @author 周旭光
		* @date 2015年6月29日
		* @note 
		*/
		void SetRGBA(float fRed,float fGreen,float fBlue ,float fAlpha = 1.0);

	private:
		float m_fRed;
		float m_fGreen;
		float m_fBlue;
		float m_fAlpha;
	};


}

#endif // end of __SYSCOLOR_H_196793B0_415A_45A4_94AD_E48A1AB62395__