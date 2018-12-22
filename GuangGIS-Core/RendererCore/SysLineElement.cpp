#include "SysLineElement.h"
#include "SysCoordinate.h"
#include "SysEnvelope.h"

namespace SysDraw
{

	SysLineElement::SysLineElement(void):SysGraphElement(type_PolyLine)
	{
		m_poPoints = NULL;
		m_nPointCount = 0;
	}

	SysLineElement::~SysLineElement(void)
	{
		RemoveAllPoints();
	}

	void SysLineElement::AddPoint(double dx,double dy)
	{
		if (NULL == m_poPoints)
		{
			m_poPoints = (SysCoordinate *)malloc(sizeof(SysCoordinate)*1);
			m_poPoints[0].x = dx;
			m_poPoints[0].y = dy;
		}

		else
		{
			m_poPoints = (SysCoordinate *)realloc(m_poPoints,sizeof(SysCoordinate)*(m_nPointCount+1));
			m_poPoints[m_nPointCount].x = dx;
			m_poPoints[m_nPointCount].y = dy;
		}

		m_nPointCount += 1;
	}

	void SysLineElement::RemoveAllPoints()
	{
		if (m_poPoints != NULL)
		{
			free(m_poPoints);
			m_poPoints = NULL;
		}

		m_nPointCount = 0;
	}

	bool SysLineElement::GetPoint(int nIndex,double &dx,double &dy) const
	{
		if (nIndex < 0 || nIndex > m_nPointCount-1)
		{
			return false;
		}

		dx = m_poPoints[nIndex].x;
		dy = m_poPoints[nIndex].y;

		return true;
	}

	SysCoordinate* SysLineElement::GetPoints() const
	{
		return m_poPoints;
	}

	int SysLineElement::GetPointCount() const
	{
		return m_nPointCount;
	}

	bool SysLineElement::Move(double dx, double dy)
	{
		for (int i = 0; i < m_nPointCount; i ++)
		{
			m_poPoints[i].x += dx;
			m_poPoints[i].y += dy;
		}

		return true;
	}

	bool SysLineElement::MovePoint(int nIndex, double dx, double dy)
	{
		if (nIndex < 0 || nIndex > m_nPointCount-1)
		{
			return false;
		}

		m_poPoints[nIndex].x += dx;
		m_poPoints[nIndex].y += dy;
		return true;
	}

	bool SysLineElement::MovePointTo(int nIndex, double dbX, double dbY)
	{
		if (nIndex < 0 || nIndex > m_nPointCount-1)
		{
			return false;
		}

		m_poPoints[nIndex].x = dbX;
		m_poPoints[nIndex].y = dbY;
		return true;
	}

	SysEnvelope& SysLineElement::GetEnvelope() const
	{
		double      dfMinX, dfMinY, dfMaxX, dfMaxY;

		SysEnvelope env;

		int nPointCount = m_nPointCount;
		if( 0 == nPointCount )
		{
			env.Init(0,0,0,0);
			return env;
		}

		dfMinX = dfMaxX = m_poPoints[0].x;
		dfMinY = dfMaxY = m_poPoints[0].y;

		for( int iPoint = 1; iPoint < nPointCount; iPoint ++ )
		{
			if( dfMaxX < m_poPoints[iPoint].x )
				dfMaxX = m_poPoints[iPoint].x;
			if( dfMaxY < m_poPoints[iPoint].y )
				dfMaxY = m_poPoints[iPoint].y;
			if( dfMinX > m_poPoints[iPoint].x )
				dfMinX = m_poPoints[iPoint].x;
			if( dfMinY > m_poPoints[iPoint].y )
				dfMinY = m_poPoints[iPoint].y;
		}

		env.Init(dfMinX,dfMaxX,dfMinY,dfMaxY);

		return env;
	}

	//点到直线的距离
	static double PtToLine(double x1,double y1,double x2,double y2,double x,double y)
	{
		return fabs((double)x*(y2-y1)+y*(x1-x2)+(y1*x2-x1*y2))/sqrt(pow((y2-y1),2.0)+pow((x1-x2),2.0));
	}

	bool SysLineElement::PointInArea(double x,double y) const
	{
		int nNUm = m_nPointCount;
		if (nNUm > 1)
		{
			for (int i=1;i<nNUm;i++)
			{
				double v = PtToLine(m_poPoints[i-1].x,m_poPoints[i-1].y,m_poPoints[i].x,m_poPoints[i].y,x,y);
				if (v <= 0.0001)
				{
					if ( (x-m_poPoints[i-1].x)*(x-m_poPoints[i].x)<=0 && (y-m_poPoints[i-1].y)*(y-m_poPoints[i].y)<=0)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void SysLineElement::GetCenterPoint(double &x,double &y) const
	{
		return;
	}

	double SysLineElement::GetPerimeter() const
	{
		double dlength = 0; 

		if (m_nPointCount > 1)
		{			
			for (int i=0; i < m_nPointCount; i++)
			{
				int tmp = i+1;
				if (tmp == m_nPointCount)
				{
					break;
				}
				dlength += sqrt(pow((m_poPoints[i].x - m_poPoints[tmp].x),2.0) + pow((m_poPoints[i].y - m_poPoints[tmp].y),2.0));
			}
		}

		return dlength;
	}

}
