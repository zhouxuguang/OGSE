
#include <assert.h>
#include "SysGraphElement.h"
#include "SysRectElement.h"
#include "SysLineElement.h"
#include "SysPolygonElement.h"
#include "SysCycleElement.h"

namespace SysDraw
{

	SysGraphElement::SysGraphElement(GraphicsElementType eGraType)
	{
		m_GraphicType = eGraType;
	}

	SysGraphElement::~SysGraphElement(void)
	{
	}

	SysGraphElement*SysGraphElement::Clone() const
	{
		return NULL;
	}

	GraphicsElementType SysGraphElement::GetElementType() const
	{
		return m_GraphicType;
	}


	bool SysGraphElement::MovePoint(int nIndex, double dx, double dy)
	{
		return false;
	}

	bool SysGraphElement::MovePointTo(int nIndex, double dbX, double dbY)
	{
		assert(false);
		return false;
	}

	void SysGraphElement::SetAlpha(int nAlpha)
	{
		m_nAlphaPrecent = nAlpha;
	}

	int SysGraphElement::GetAlpha() const
	{
		return m_nAlphaPrecent;
	}

	bool SysGraphElement::PointInArea(double x,double y) const
	{
		assert(false);
		return false;
	}

	void SysGraphElement::GetCenterPoint(double &x,double &y) const
	{
		assert(false);
		return ;
	}

	double SysGraphElement::GetPerimeter() const
	{
		return 0;
	}

	double SysGraphElement::GetArea() const
	{
		return 0;
	}

	SysGraphElement* CreateGraphElement(GraphicsElementType eGraphicType)
	{
		if (eGraphicType == type_PolyLine)
		{
			return new(std::nothrow) SysLineElement;
		}

		else if (eGraphicType == type_Rect)
		{
			return new(std::nothrow) SysRectElement;
		}

		else if (eGraphicType == type_Ploygon)
		{
			return new(std::nothrow) SysPolygonElement;
		}

		else if (eGraphicType == type_Cycle)
		{
			return new(std::nothrow) SysCycleElement;
		}

		return NULL;
	}

	void DestroyGraphElement(SysGraphElement* pGraphElem)
	{
		if (pGraphElem != NULL)
		{
			delete pGraphElem;
			pGraphElem = NULL;
		}
	}

}
