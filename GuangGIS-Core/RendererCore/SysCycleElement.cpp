
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "SysCycleElement.h"
#include "SysCoordinate.h"
#include "SysEnvelope.h"

namespace SysDraw
{

	SysCycleElement::SysCycleElement(void) : SysGraphElement(type_Cycle)
	{
		m_pCenterPoint = new(std::nothrow) SysCoordinate(0,0);
		m_dbRadius = 0.0;
	}

	SysCycleElement::~SysCycleElement(void)
	{
		if (m_pCenterPoint != NULL)
		{
			delete m_pCenterPoint;
			m_pCenterPoint = NULL;
		}

		m_dbRadius = 0.0;
	}

	bool SysCycleElement::Move( double dx, double dy )
	{
		double dbGeoX = m_pCenterPoint->x;
		double dbGeoY = m_pCenterPoint->y;

		dbGeoX += dx;
		dbGeoY += dy;
		m_pCenterPoint->SetX(dbGeoX);
		m_pCenterPoint->SetY(dbGeoY);

		return true;
	}

	SysEnvelope& SysCycleElement::GetEnvelope( void ) const
	{
		SysEnvelope env;
		double dbMinx = m_pCenterPoint->x - m_dbRadius;
		double dbMiny = m_pCenterPoint->y - m_dbRadius;

		double dbMaxx = m_pCenterPoint->x + m_dbRadius;
		double dbMaxy = m_pCenterPoint->y + m_dbRadius;
		env.Init(dbMinx,dbMaxx,dbMiny,dbMaxy);
		return env;
	}

	bool SysCycleElement::PointInArea( double x,double y ) const
	{
		double dbLength = (x - m_pCenterPoint->x) * (x - m_pCenterPoint->x) + (y - m_pCenterPoint->y) * (y - m_pCenterPoint->y);
		return dbLength <= m_dbRadius * m_dbRadius;
	}

	void SysCycleElement::GetCenterPoint( double &x,double &y ) const
	{
		x = m_pCenterPoint->x;
		y = m_pCenterPoint->y;
	}

	double SysCycleElement::GetPerimeter() const
	{
		return 2 * M_PI * m_dbRadius;
	}

	double SysCycleElement::GetArea() const
	{
		return M_PI * m_dbRadius * m_dbRadius;
	}

	void SysCycleElement::SetCenterPoint( const double dbX,const double dbY )
{
		m_pCenterPoint->SetX(dbX);
		m_pCenterPoint->SetY(dbY);
	}

	void SysCycleElement::SetRadius( const double dbRadius )
	{
		m_dbRadius = dbRadius;
	}
}
