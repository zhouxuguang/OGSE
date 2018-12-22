#include "SysRectElement.h"
#include "SysCoordinate.h"
#include "SysEnvelope.h"

namespace SysDraw
{

	SysRectElement::SysRectElement(void) :SysGraphElement(type_Rect)
	{
		m_pLeftTop = new SysCoordinate;
		m_pRightBottom = new SysCoordinate;
	}

	SysRectElement::~SysRectElement(void)
	{
		delete m_pLeftTop;
		m_pLeftTop = NULL;
		delete m_pRightBottom;
		m_pRightBottom = NULL;
	}

	void SysRectElement::Init(double x1,double x2,double y1,double y2)
	{
		if (x1 > x2)
		{
			m_pLeftTop->x = x2;
			m_pRightBottom->x = x1;
		}
		else
		{
			m_pLeftTop->x = x1;
			m_pRightBottom->x = x2;
		}

		if (y1 > y2)
		{
			m_pLeftTop->y = y1;
			m_pRightBottom->y = y2;
		}
		else
		{
			m_pLeftTop->y = y2;
			m_pRightBottom->y = y1;
		}
	}

	bool SysRectElement::Move(double dx, double dy)
	{
		m_pLeftTop->x += dx;
		m_pLeftTop->y += dy;
		m_pRightBottom->x += dx;
		m_pRightBottom->y += dy;

		return true;
	}

	SysEnvelope& SysRectElement::GetEnvelope() const
	{
		SysEnvelope env;
		env.Init(m_pLeftTop->x,m_pRightBottom->x,m_pLeftTop->y,m_pRightBottom->y);

		return env;
	}

	bool SysRectElement::PointInArea(double x,double y) const
	{
		SysEnvelope env = GetEnvelope();
		if ( x >=env.minX && x <= env.maxX && y >= env.minY && y<= env.maxY )
		{
			return true;
		}

		return false;
	}

	void SysRectElement::GetCenterPoint(double &x,double &y) const
	{
		x = (m_pLeftTop->x + m_pRightBottom->x)/2.0;
		y = (m_pRightBottom->y + m_pLeftTop->y)/2.0;
	}

	double SysRectElement::GetPerimeter() const
	{
		double dlength = 0;
		dlength = (fabs((double)m_pLeftTop->x - m_pRightBottom->x) + fabs(m_pRightBottom->y - m_pLeftTop->y))*2;
		return dlength;
	}

	double SysRectElement::GetArea() const
	{
		double dArea = 0;
		dArea = fabs(m_pLeftTop->x - m_pRightBottom->x) * fabs(m_pRightBottom->y - m_pLeftTop->y);
		return dArea;
	}

	double SysRectElement::GetMinX() const
	{
		return /*std::*/min(m_pLeftTop->x,m_pRightBottom->x);
	}

	double SysRectElement::GetMaxX() const
	{
		return /*std::*/max(m_pLeftTop->x,m_pRightBottom->x);
	}

	double SysRectElement::GetMinY() const
	{
		return /*std::*/min(m_pLeftTop->y,m_pRightBottom->y);
	}

	double SysRectElement::GetMaxY() const
	{
		return /*std::*/max(m_pLeftTop->y,m_pRightBottom->y);
	}

}
