/**
* @file              SysFeatureRenderer.h
* @brief             矢量数据渲染器基类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年7月27日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSFEATURERENDERER_H_C54B6C89_5768_41FF_BB9D_E11F210F6C18__
#define __SYSFEATURERENDERER_H_C54B6C89_5768_41FF_BB9D_E11F210F6C18__

#include "SysDrawCommon.h"

namespace SysDraw
{
	enum Render_Type
	{
		SINGLE_SYMBOL_RENDERER,			//单一符号渲染
		UNIQUE_VALUE_RENDERER,			//唯一值渲染
		CLASS_BREAK_RENDERER			//分级符号渲染
	};

	typedef list<void*> SysSymbolList;

	class SysRenderContext;
	class SysFeatureLayer;
	class SysSymbol;
	class SysFeature;

	class SYSDRAW_API SysFeatureRenderer
	{
	public:
		SysFeatureRenderer(void);

		virtual ~SysFeatureRenderer(void);

		/**
		* @brief 获得默认的渲染器
		* @param[in] int nGeomType
		* @return SysFeatureRenderer*
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		static SysFeatureRenderer* GetDefaultRenderer( int nGeomType );

		/**
		* @brief 获得默认的符号
		* @param[in] int nGeomType
		* @return SysSymbol*
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		static SysSymbol* GetDefaultSymbol(int nGeomType);

		/**
		* @brief 获得渲染器的名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererName() const = 0;

		/**
		* @brief 获得渲染器的中文名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererChsName() const = 0;

		/**
		* @brief 返回渲染器类型
		* @return Render_Type
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual Render_Type GetRendererType() const = 0;

		/**
		* @brief 克隆当前渲染器
		* @return SysFeatureRenderer*
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual SysFeatureRenderer* Clone() const = 0;

		/**
		* @brief 开始渲染函数
		* @param[in]  SysRenderContext* pCcontext
		* @param[in] const SysFeatureLayer *vlayer
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual void BeginRender( SysRenderContext* pCcontext, const SysFeatureLayer *vlayer ){};

		/**
		* @brief 结束渲染
		* @param[in] SysRenderContext* pCcontext
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual void EndRender( SysRenderContext* pCcontext ) {};

		/**
		* @brief 获得符号列表
		* @return virtualconst 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual const SysSymbolList GetSymbols() const = 0;

		/**
		* @brief 根据FID获得要绘制的矢量符号
		* @param[in] SysFeature* pFeature
		* @return SysSymbol*
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual SysSymbol* GetSymbol(SysFeature* pFeature) const = 0;
	};


	/**
	* @brief 创建渲染器
	* @param[in] Render_Type eType
	* @return SysFeatureRenderer*
	* @author 周旭光
	* @date 2015年7月27日
	* @note 
	*/
	SYSDRAW_API SysFeatureRenderer* CreateFeatureRenderer(Render_Type eType);

	/**
	* @brief 销毁渲染器
	* @param[in] SysFeatureRenderer* pRender
	* @return void
	* @author 周旭光
	* @date 2015年7月27日
	* @note 
	*/
	SYSDRAW_API void DestroyFeatureRenderer(SysFeatureRenderer* pRender);

}

#endif // end of __SYSFEATURERENDERER_H_C54B6C89_5768_41FF_BB9D_E11F210F6C18__
