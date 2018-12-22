#include "SysLineSymbol.h"

namespace SysDraw
{

	SysLineSymbol::SysLineSymbol(void)
	{
	}

	SysLineSymbol::~SysLineSymbol(void)
	{
	}

	SysDraw::SysSymbolType SysLineSymbol::GetSymbolType() const
	{
		return LineSymbol;
	}
}
