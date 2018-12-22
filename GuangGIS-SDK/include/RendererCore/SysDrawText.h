/**
* @file              SysDrawText.h
* @brief             文字绘制类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月25日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSDRAWTEXT_H_A19326D9_F275_4C37_B0F8_484BEBF64CF0__
#define __SYSDRAWTEXT_H_A19326D9_F275_4C37_B0F8_484BEBF64CF0__

#include "SysDrawCommon.h"

namespace SysDraw
{
	class SysRGBAColor;

	class SYSDRAW_API SysDrawText
	{
	public:
		explicit SysDrawText(const char* pszFontFile);

		~SysDrawText();

		/**
		* @brief 设置字体颜色
		* @param[in] const SysRGBAColor& colorText
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void SetTextColor(const SysRGBAColor& colorText);

		/**
		* @brief 设置字体颜色
		* @param[in] float fRed
		* @param[in] float fGreen
		* @param[in] float fBlue
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void SetTextColor(float fRed,float fGreen,float fBlue);

		/**
		* @brief 获得字体颜色
		* @return SysRGBAColor&
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		SysRGBAColor& GetTextColor() const;

		/**
		* @brief 设置字体宽度
		* @param[in] int nTextWidth
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void SetTextWidth(int nTextWidth);

		/**
		* @brief 获得字体宽度
		* @return int
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		int GetTextWidth() const;

		/**
		* @brief 设置字体高度
		* @param[in] int nTextHeight
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void SetTextHeight(int nTextHeight);

		/**
		* @brief 获得字体高度
		* @return int
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		int GetTextHeight() const;

		/**
		* @brief 绘制文字
		* @param[in] const char* pszText
		* @param[in] int x
		* @param[in] int y
		* @param[in] int nMaxWidth
		* @param[in] int nHeight
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void DrawTextExt(const char* pszText,int x , int y, int nMaxWidth , int nHeight) const;

		/**
		* @brief 绘制文字
		* @param[in] const wchar_t* pszText
		* @param[in] int x
		* @param[in] int y
		* @param[in] int nMaxWidth
		* @param[in] int nHeight
		* @return void
		* @author 周旭光
		* @date 2015年8月25日
		* @note 
		*/
		void DrawTextExt(const wchar_t* pszText,int x , int y, int nMaxWidth , int nHeight) const;

	private:
		SysRGBAColor* m_pTextColor;			//文本颜色
		char* m_pszFontFile;				//字体文件路径
		int m_nTextWidth;					//字体宽度
		int m_nTextHeight;					//字体高度
	};
}

#endif // end of __SYSDRAWTEXT_H_A19326D9_F275_4C37_B0F8_484BEBF64CF0__
