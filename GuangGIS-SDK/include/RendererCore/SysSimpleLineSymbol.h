/**
* @file              SysSimpleLineSymbol.h
* @brief             简单线符号
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSSIMPLELINESYMBOL_H_840FA1AF_1D69_459A_8651_A800881C9A19__
#define __SYSSIMPLELINESYMBOL_H_840FA1AF_1D69_459A_8651_A800881C9A19__

#include "SysLineSymbol.h"

namespace SysDraw
{
	enum SysLineStyle
	{
		Solid_line,			//实线
		Dashed_Line,		//
		Dotted_Line,
		DashDot_Line
	};

	class SYSDRAW_API SysSimpleLineSymbol : public SysLineSymbol
	{
	public:
		SysSimpleLineSymbol(void);

		~SysSimpleLineSymbol(void);

		void SetLineStyle(SysLineStyle eStyle);

		virtual SysSymbolType GetSymbolType() const ;

		virtual void RenderFeature(const SysFeature* pFeature,SysRenderContext* pContext, bool bHighLight);

	private:
		SysLineStyle m_eLineStyle;
	};

}

#endif // end of __SYSSIMPLELINESYMBOL_H_840FA1AF_1D69_459A_8651_A800881C9A19__
