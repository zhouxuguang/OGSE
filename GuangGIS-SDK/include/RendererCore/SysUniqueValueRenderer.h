/**
* @file              SysUniqueValueRenderer.h
* @brief             唯一值渲染器
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月18日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSUNIQUEVALUERENDERER_H_16D1BF80_6064_4BF9_BAE0_7A7064B06C56__
#define __SYSUNIQUEVALUERENDERER_H_16D1BF80_6064_4BF9_BAE0_7A7064B06C56__

#include "SysFeatureRenderer.h"

namespace SysDraw
{
	class SysSymbol;

	class SYSDRAW_API SysUniqueValueRenderer : public SysFeatureRenderer
	{
		friend SysFeatureRenderer* CreateFeatureRenderer(Render_Type eType);

	private:
		SysUniqueValueRenderer(void);

		~SysUniqueValueRenderer(void);

	public:

		/**
		* @brief 获得渲染器的名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererName() const ;

		/**
		* @brief 获得渲染器的中文名字
		* @return std::string 
		* @author 周旭光
		* @date 2015年7月27日
		* @note 
		*/
		virtual std::string GetRendererChsName() const ;

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
		virtual SysFeatureRenderer* Clone() const ;

		/**
		* @brief 渲染函数
		* @param[in]  SysRenderContext* pContext
		* @param[in] const SysFeatureLayer *pLayer
		* @return virtualvoid 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual void BeginRender( SysRenderContext* pContext, const SysFeatureLayer *pLayer );

		virtual void EndRender( SysRenderContext* pContext );

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
		* @return virtualSysSymbol* 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		virtual SysSymbol* GetSymbol(SysFeature* pFeature) const ;

		/**
		* @brief 设置分类的字段索引
		* @param[in] int nFieldIndex
		* @return void
		* @author 周旭光
		* @date 2015年8月19日
		* @note 
		*/
		void SetClassificationField( int nFieldIndex );

		/**
		* @brief 获得分类的字段索引
		* @return int
		* @author 周旭光
		* @date 2015年8月19日
		* @note 
		*/
		int GetClassificationField() const;

		/**
		* @brief 插入唯一值和对应的符号
		* @param[in]  std::string strValue
		* @param[in] SysSymbol* pSymbol
		* @return void
		* @author 周旭光
		* @date 2015年8月19日
		* @note 
		*/
		void InsertValue( std::string strValue, SysSymbol* pSymbol );

	private:
		int								m_nFieldIndex;			//分类的字段索引
		std::map<std::string,SysSymbol* >	m_mapSymbols;	//唯一值对应的符号

		unsigned int m_nVertexBuffer;
	};

}

#endif // end of __SYSUNIQUEVALUERENDERER_H_16D1BF80_6064_4BF9_BAE0_7A7064B06C56__
