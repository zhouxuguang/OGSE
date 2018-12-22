/**
* @file              SysSimplePointSymbol.h
* @brief             简单点符号类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSIMPLEPOINTSYMBOL_H_60360038_7D7B_473A_BE11_5B837FBA7B8D__
#define __SYSSIMPLEPOINTSYMBOL_H_60360038_7D7B_473A_BE11_5B837FBA7B8D__

#include "SysPointSymbol.h"

namespace SysDraw
{
	enum SIMPLEPOINTSYMBOL_TYPE
	{
		CYCLE_POINT,
		SQUARE_POINT,
		CROSS_POINT,
		DIAMOND_POINT
	};

	class SysPoint;

	class SYSDRAW_API SysSimplePointSymbol : public SysPointSymbol
	{
	public:
		SysSimplePointSymbol(void);

		~SysSimplePointSymbol(void);

		virtual SysSymbolType GetSymbolType() const ;

		virtual void RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight);

	private:
		float m_fSize;		//符号大小
		float m_fAngle;		//旋转角度
		float m_fXOffset;	//X方向偏移
		float m_fYOffset;	//Y方向偏移
		bool m_bUseOutLine;		//是否用边框

		SIMPLEPOINTSYMBOL_TYPE m_eType;

		void Init();

		void RenderSimplePoint(SysPoint* pPoint,SysRenderContext* pContext);
	};

}

#endif // end of __SYSSIMPLEPOINTSYMBOL_H_60360038_7D7B_473A_BE11_5B837FBA7B8D__
