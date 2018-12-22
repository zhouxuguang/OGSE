#include "SysPointSymbol.h"

namespace SysDraw
{

	SysPointSymbol::SysPointSymbol(void)
	{
	}

	SysPointSymbol::~SysPointSymbol(void)
	{
	}

	SysDraw::SysSymbolType SysPointSymbol::GetSymbolType() const
	{
		return PointSymbol;
	}
}
