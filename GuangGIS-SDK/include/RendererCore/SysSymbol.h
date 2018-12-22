/**
* @file              SysSymbol.h
* @brief             符号基类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSYMBOL_H_DC0A9E7A_AF2D_41B9_AB88_B461B20BFBA4__
#define __SYSSYMBOL_H_DC0A9E7A_AF2D_41B9_AB88_B461B20BFBA4__

#include "SysDrawCommon.h"


namespace SysDraw
{
	enum SysSymbolType
	{
		PointSymbol,		//点符号
		LineSymbol,			//线状符号
		FillSymbol			//填充符号
	};

	enum SysUnitType
	{
		Point_Unit,			//像素点
		CentiMeter_Unit,	//分米
		MiliMeter_Unit		//厘米
	};

	class SysFeature;
	class SysRGBAColor;
	class SysRenderContext;

	class SYSDRAW_API SysSymbol
	{
	public:
		SysSymbol(void);

		virtual ~SysSymbol(void);

		/**
		* @brief GetSymbolType
		* @return SysSymbolType
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		virtual SysSymbolType GetSymbolType() const = 0;

		/**
		* @brief GetUnitType
		* @return SysUnitType
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		SysUnitType GetUnitType() const;

		/**
		* @brief SetUnitType
		* @param[in] SysUnitType eUnitType
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		void SetUnitType(SysUnitType eUnitType);

		/**
		* @brief GetOutLineColor
		* @return SysRGBAColor&
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		SysRGBAColor& GetOutLineColor() const;

		/**
		* @brief SetOutLineColor
		* @param[in] const SysRGBAColor& color
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		void SetOutLineColor(const SysRGBAColor& color);

		/**
		* @brief GetColor
		* @return SysRGBAColor&
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		SysRGBAColor& GetColor() const;

		/**
		* @brief SetColor
		* @param[in] const SysRGBAColor& color
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		void SetColor(const SysRGBAColor& color);

		/**
		* @brief GetOutLineWidth
		* @return float
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		float GetOutLineWidth() const;

		/**
		* @brief SetOutLineWidth
		* @param[in] float fWidth
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		void SetOutLineWidth(float fWidth);

		/**
		* @brief 绘制要素
		* @param[in] const SysFeature* pFeature
		* @param[in] SysRenderContext* pContext
		* @param[in] bool bHighLight 是否高亮显示
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		virtual void RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight){};

	protected:
		SysUnitType m_UnitType;
		SysRGBAColor *m_pOutLineColor;		//外边框颜色
		SysRGBAColor *m_pColor;				//内部填充色
		float m_fOutLineWidth;				//外边框宽度
	};

}

#endif // end of __SYSSYMBOL_H_DC0A9E7A_AF2D_41B9_AB88_B461B20BFBA4__
