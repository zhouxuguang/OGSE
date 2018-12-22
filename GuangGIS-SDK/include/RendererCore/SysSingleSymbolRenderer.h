/**
* @file              SysSingleSymbolRenderer.h
* @brief             单一符号渲染
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSINGLESYMBOLRENDERER_H_9A782E36_0E98_450F_95E2_EE93DDF114D1__
#define __SYSSINGLESYMBOLRENDERER_H_9A782E36_0E98_450F_95E2_EE93DDF114D1__

#include "SysFeatureRenderer.h"

namespace SysDraw
{
	class SysSymbol;

	class SYSDRAW_API SysSingleSymbolRenderer : public SysFeatureRenderer
	{
		friend SysFeatureRenderer* CreateFeatureRenderer(Render_Type eType);

	private:
		SysSingleSymbolRenderer(void);

		~SysSingleSymbolRenderer(void);

	public:

		/**
		* @brief 获得渲染器的名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererName() const;

		/**
		* @brief 获得渲染器的名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererChsName() const;

		/**
		* @brief 返回渲染器类型
		* @return Render_Type
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual Render_Type GetRendererType() const;

		/**
		* @brief 克隆当前渲染器
		* @return SysFeatureRenderer*
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual SysFeatureRenderer* Clone() const;

		virtual void BeginRender( SysRenderContext* pCcontext, const SysFeatureLayer *vlayer );

		/**
		* @brief 获得符号列表
		* @return virtualconst 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual const SysSymbolList GetSymbols() const;

		/**
		* @brief GetSymbol
		* @param[in] SysFeature* pFeature
		* @return SysSymbol*
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual SysSymbol* GetSymbol(SysFeature* pFeature) const;

		/**
		* @brief 设置符号
		* @param[in] SysSymbol* pSymbol
		* @return void
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		void SetSymbol(SysSymbol* pSymbol);

	private:
		SysSymbol* m_pSymbol;

		unsigned int m_nVertexBuffer;
	};

}

#endif // end of __SYSSINGLESYMBOLRENDERER_H_9A782E36_0E98_450F_95E2_EE93DDF114D1__
