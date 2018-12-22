#include "SysPolygonElement.h"
#include "SysLineElement.h"

namespace SysDraw
{

	SysPolygonElement::SysPolygonElement(void) : SysGraphElement(type_Ploygon)
	{
		m_pExteriorRing = new(std::nothrow) SysLineElement;
	}

	SysPolygonElement::~SysPolygonElement(void)
	{
		if (m_pExteriorRing != NULL)
		{
			delete m_pExteriorRing;
			m_pExteriorRing = NULL;
		}
	}

	void SysPolygonElement::AddPoint(double dx,double dy)
	{
		m_pExteriorRing->AddPoint(dx,dy);
	}

	void SysPolygonElement::AddIsLand(SysLineElement* pLineRing)
	{
		m_vecIsLands.push_back(pLineRing);
	}

	bool SysPolygonElement::Move(double dx, double dy)
	{
		m_pExteriorRing->Move(dx,dy);

		int nCount = m_vecIsLands.size();
		for (int i = 0; i < nCount; i ++)
		{
			m_vecIsLands[i]->Move(dx,dy);
		}

		return true;
	}

	bool SysPolygonElement::MovePoint(int nIndex, double dx, double dy)
	{
		return m_pExteriorRing->MovePoint(nIndex,dx,dy);
	}

	bool SysPolygonElement::MovePointTo(int nIndex, double dbX, double dbY)
	{
		return m_pExteriorRing->MovePointTo(nIndex,dbX,dbY);
	}

	SysEnvelope& SysPolygonElement::GetEnvelope() const
	{
		return m_pExteriorRing->GetEnvelope();
	}

	bool SysPolygonElement::PointInArea(double x,double y) const
	{
		return false;
	}

	void SysPolygonElement::GetCenterPoint(double &x,double &y) const
	{
		//
	}

	double SysPolygonElement::GetPerimeter() const
	{
		return m_pExteriorRing->GetPerimeter();
	}

	double SysPolygonElement::GetArea() const
	{
		double dbArea = m_pExteriorRing->GetArea();

		int nCount = m_vecIsLands.size();
		for (int i = 0; i < nCount; i ++)
		{
			dbArea -= m_vecIsLands[i]->GetArea();
		}
		return dbArea;
	}

	int SysPolygonElement::GetIsLandCount() const
	{
		return m_vecIsLands.size();
	}

	SysLineElement* SysPolygonElement::GetIsLand(int nIndex) const
	{
		int nCount = m_vecIsLands.size();
		if (nIndex < 0 || nIndex >= nCount)
		{
			return NULL;
		}

		return m_vecIsLands[nIndex];
	}

	SysLineElement* SysPolygonElement::GetExteriorRing() const
	{
		return m_pExteriorRing;
	}

}
