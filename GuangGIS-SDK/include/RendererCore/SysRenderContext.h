/**
* @file              SysRenderContext.h
* @brief             渲染上下文
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月8日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSRENDERCONTEXT_H_52CC6321_9DCD_4983_8D2C_8AAEB9795EEF__
#define __SYSRENDERCONTEXT_H_52CC6321_9DCD_4983_8D2C_8AAEB9795EEF__

#include "SysEnvelope.h"
#include "SysDisplayTransform.h"


namespace SysDraw
{

	class SYSDRAW_API SysRenderContext
	{
	public:
		SysRenderContext(void);
		~SysRenderContext(void);

		/**
		* @brief GetExtent
		* @return const SysEnvelope& 
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		const SysEnvelope& GetExtent() const;

		/**
		* @brief GetExtent
		* @return SysEnvelope&
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		SysEnvelope& GetExtent();

		/**
		* @brief GetDisplayTran
		* @return const SysDisplayTransform& 
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		const SysDisplayTransform& GetDisplayTran() const;

		/**
		* @brief GetDisplayTran
		* @return SysDisplayTransform&
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		SysDisplayTransform& GetDisplayTran();

		/**
		* @brief SetExtent
		* @param[in] const SysEnvelope& extent
		* @return void
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		void SetExtent(const SysEnvelope& extent);

		/**
		* @brief SetDisplayTran
		* @param[in] const SysDisplayTransform& displayTran
		* @return void
		* @author 周旭光
		* @date 2015年7月8日
		* @note 
		*/
		void SetDisplayTran(const SysDisplayTransform& displayTran);


	private:
		SysEnvelope				m_Extent;			//绘制的地理范围
		SysDisplayTransform		m_DisplayTran;		//坐标显示转换参数
	};

}

#endif // end of __SYSRENDERCONTEXT_H_52CC6321_9DCD_4983_8D2C_8AAEB9795EEF__
