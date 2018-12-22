#include "SysFillSymbol.h"


namespace SysDraw
{


	SysFillSymbol::SysFillSymbol(void)
	{
	}

	SysFillSymbol::~SysFillSymbol(void)
	{
	}

	SysDraw::SysSymbolType SysFillSymbol::GetSymbolType() const
	{
		return FillSymbol;
	}
}
