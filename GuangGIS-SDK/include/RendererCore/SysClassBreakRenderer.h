/**
* @file              SysClassBreakRenderer.h
* @brief             分级渲染器
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月24日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSCLASSBREAKRENDERER_H_4602B3A4_CA3A_4E66_B8DD_8DD9FF1E38E4__
#define __SYSCLASSBREAKRENDERER_H_4602B3A4_CA3A_4E66_B8DD_8DD9FF1E38E4__

#include "SysFeatureRenderer.h"
#include "SysValueStatistics.h"

namespace SysDraw
{

	class SYSDRAW_API SysClassBreakRenderer : public SysFeatureRenderer
	{
		friend SysFeatureRenderer* CreateFeatureRenderer(Render_Type eType);

	private:
		SysClassBreakRenderer(void);

		~SysClassBreakRenderer(void);

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
		* @return void
		* @author 周旭光
		* @date 2015年10月10日
		* @note 
		*/
		virtual void BeginRender( SysRenderContext* pContext, const SysFeatureLayer *pLayer );

		virtual void EndRender( SysRenderContext* pContext );

		/**
		* @brief 获得符号列表
		* @return virtualconst 
		* @author 周旭光
		* @date 2015年8月19日
		* @note 
		*/
		virtual const SysSymbolList GetSymbols() const;

		/**
		* @brief GetSymbolByFid
		* @param[in] int nFid
		* @return SysSymbol*
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
		* @brief SetMinClassNum
		* @param[in] int nMinClassNum
		* @return void
		* @author 周旭光
		* @date 2015年8月26日
		* @note 
		*/
		void SetMinClassNum(int nMinClassNum);

		/**
		* @brief GetMinClassNum
		* @return int
		* @author 周旭光
		* @date 2015年8月26日
		* @note 
		*/
		int GetMinClassNum() const;

		/**
		* @brief 设置分类数目
		* @param[in] int nClassNum
		* @return void
		* @author 周旭光
		* @date 2015年8月26日
		* @note 
		*/
		void SetClassNum(int nClassNum);

		/**
		* @brief 获得分类数目
		* @return int
		* @author 周旭光
		* @date 2015年8月26日
		* @note 
		*/
		int GetClassNum() const;

		/**
		* @brief SortClassAscending
		* @return int
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		int SortClassAscending() const;

		/**
		* @brief 获得分级方式
		* @return ClassBreak_Type
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		ClassBreak_Type GetClassBreakType() const;

		/**
		* @brief 设置分级方式
		* @param[in] ClassBreak_Type eBreakType
		* @return void
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		void SetClassBreakType(ClassBreak_Type eBreakType);

		/**
		* @brief 获得每一个分级的上限和下限
		* @param[in] double* pValues
		* @param[in] int nCount
		* @return void
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		void GetClassIntervals(double* pValues,int nCount) const;

		/**
		* @brief 获得分级的间隔值
		* @return constdouble* 
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		const double* GetIntervalValues() const;

		/**
		* @brief 以分级的最小值作为键值查找是否在符号列表中
		* @param[in] double dbKey
		* @return bool
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		bool FindKey(double dbKey) const;

		/**
		* @brief 插入符号
		* @param[in] double dbMinValue
		* @param[in] SysSymbol* pSymbol
		* @return void
		* @author 周旭光
		* @date 2015年11月13日
		* @note 
		*/
		void InsertSymbol(double dbMinValue,SysSymbol* pSymbol);

	private:
		int								m_nClassFieldIndex;				//分级的字段
		int								m_nMinClassNum;					//最小的分级数目
		int								m_nClassNum;					//分级的总数
		ClassBreak_Type					m_eBreakType;					//分级的方式

		mutable double *				m_pIntervals;					//间隔值，分级中间的间隔值，包括两端的最大和最小值

		typedef std::map<double,SysSymbol *> BreakSymbolMap;			//键值是分级的下限
		BreakSymbolMap					m_mapSymbols;

	};

}

#endif // end of __SYSCLASSBREAKRENDERER_H_4602B3A4_CA3A_4E66_B8DD_8DD9FF1E38E4__
