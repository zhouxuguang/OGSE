#include "SysSymbol.h"
#include "SysRGBColor.h"

namespace SysDraw
{

	SysSymbol::SysSymbol(void)
	{
		m_UnitType = Point_Unit;
		m_pOutLineColor = new SysRGBAColor(1,1,1);
		m_pColor = new SysRGBAColor(1,1,1);
	}

	SysSymbol::~SysSymbol(void)
	{
		if (m_pOutLineColor != NULL)
		{
			delete m_pOutLineColor;
			m_pOutLineColor = NULL;
		}

		if (m_pColor != NULL)
		{
			delete m_pColor;
			m_pColor = NULL;
		}
	}

	SysUnitType SysSymbol::GetUnitType() const
	{
		return m_UnitType;
	}

	void SysSymbol::SetUnitType(SysUnitType eUnitType)
	{
		m_UnitType = eUnitType;
	}

	SysRGBAColor& SysSymbol::GetOutLineColor()  const
	{
		return *m_pOutLineColor;
	}

	void SysSymbol::SetOutLineColor(const SysRGBAColor& color)
	{
		*m_pOutLineColor = color;
	}

	SysRGBAColor& SysSymbol::GetColor()  const
	{
		return *m_pColor;
	}

	void SysSymbol::SetColor(const SysRGBAColor& color)
	{
		*m_pColor = color;
	}

	float SysSymbol::GetOutLineWidth() const
	{
		return m_fOutLineWidth;
	}

	void SysSymbol::SetOutLineWidth(float fWidth)
	{
		m_fOutLineWidth = fWidth;
	}

}

