#include "CG_Envelope.h"


CG_Envelope::CG_Envelope()
{
	minX = 0;
	minY = -2;
	maxX = 0;
	maxY = -2;
}

//锟斤拷锟斤拷锟侥癸拷锟届函锟斤拷
CG_Envelope::CG_Envelope(double minX,double maxX,double minY,double maxY)
{
	//锟斤拷要锟饺斤拷锟斤拷锟斤拷小锟斤拷然锟斤拷锟劫斤拷锟斤拷锟叫讹拷
	if (minX > maxX)
	{
		this->minX = maxX;
		this->maxX = minX;
	}
	else
	{
		this->minX = minX;
		this->maxX = maxX;
	}

	if (minY > maxY)
	{
		this->minY = maxY;
		this->maxY = minY;
	}
	else
	{
		this->minY = minY;
		this->maxY = maxY;
	}
}

//锟斤拷锟斤拷锟斤拷锟届函锟斤拷
CG_Envelope::CG_Envelope(const CG_Envelope& envelope)
{
	this->minX = envelope.minX;
	this->maxX = envelope.maxX;
	this->minY = envelope.minY;
	this->maxY = envelope.maxY;
}

//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞硷拷锟�
CG_Envelope::CG_Envelope(const SPOINT &coord1,const SPOINT & coord2)
{
	if (coord1.x < coord2.x)
	{
		minX = coord1.x;
		maxX = coord2.x;
	}
	else
	{
		minX = coord2.x;
		maxX = coord1.x;
	}

	if (coord1.y < coord2.y)
	{
		minY = coord1.y;
		maxY = coord2.y;
	}
	else
	{
		minY = coord2.y;
		maxY = coord1.y;
	}
}

//锟斤拷锟斤拷锟斤拷锟斤拷
CG_Envelope::~CG_Envelope(void)
{
}

int CG_Envelope::InterSects(const CG_Envelope & otherEvp)
{
	//锟斤拷锟斤拷锟斤拷锟斤拷锟侥革拷锟斤拷锟斤拷
	if (maxX < otherEvp.minX || minX > otherEvp.maxX ||
		maxY < otherEvp.minY || minY > otherEvp.maxY)
	{
		return 0;
	}

	//锟侥革拷锟皆角凤拷锟斤拷
	if (maxX < otherEvp.minX && maxY < otherEvp.minY)
	{
		return 0;
	}
	if (maxX < otherEvp.minX && minY > otherEvp.maxY)
	{
		return 0;
	}

	//锟节撅拷锟斤拷锟斤拷
	if (minX >= otherEvp.minX && maxX <= otherEvp.maxX &&
		minY >= otherEvp.minY && maxY <= otherEvp.maxY)
	{
		return 1;
	}

	return 2;	//锟洁交
}

// 锟叫断撅拷锟斤拷锟角凤拷为锟斤拷
bool CG_Envelope::IsNull(void) const
{
	if (maxX < minX || maxY < minY)
	{
		return true;
	} 
	return false;
}

// 锟斤拷取锟斤拷小锟斤拷锟斤拷锟轿的匡拷锟�
double CG_Envelope::GetWidth(void) const
{
	if (IsNull())
	{
		return 0;
	}
	return fabs(maxX - minX);
}

// 锟斤拷取锟斤拷小锟斤拷锟斤拷锟轿的高讹拷
double CG_Envelope::GetHeight(void) const
{
	if (IsNull())
	{
		return 0;
	}
	return fabs(maxY - minY);
}

// 锟斤拷镁锟斤拷蔚锟斤拷锟斤拷牡锟斤拷锟斤拷
SPOINT & CG_Envelope::Center() const
{
	SPOINT pt;
	if (IsNull())
	{
		return pt;
	}

	pt.x = (minX+maxX)/2;
	pt.y = (minY+maxY)/2;
	return pt;
}

bool CG_Envelope::Center(double *pX,double *pY) const
{
	if (IsNull())
	{
		return 0;
	}
	*pX = (minX+maxX)/2;
	*pY = (minY+maxY)/2;

	return 1;
}

//锟斤拷锟斤拷锟角凤拷锟斤拷锟揭伙拷锟�BR
bool CG_Envelope::Contains(const CG_Envelope &env) const
{
	if (IsNull() || env.IsNull())
	{
		return false;
	}
	return env.minX > minX &&
		env.maxX < maxX &&
		env.minY > minY &&
		env.maxY < maxY;
}

//锟叫讹拷一锟斤拷锟斤拷锟角凤拷锟节该撅拷锟斤拷锟斤拷
bool CG_Envelope::Contains(const SPOINT &pt) const
{
	if (IsNull())
	{
		return false;
	}

	if (pt.x > minX && pt.x < maxX && pt.y > minY && pt.y < maxY)
	{
		return 1;
	}
	return 0;
}

//锟节撅拷锟斤拷锟解返锟斤拷0锟斤拷锟斤拷锟津返伙拷1
bool CG_Envelope::IsPointInRect(double x,double y) const
{
	if (IsNull())
	{
		return false;
	}

	if (x > minX && x < maxX && y > minY && y < maxY)
	{
		return 1;
	}
	return 0;
}

//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟洁交锟侥诧拷锟斤拷
CG_Envelope &CG_Envelope::Intersection(const CG_Envelope& env)
{
	if (IsNull() || env.IsNull() || ! InterSects(env))
	{
		//return new CG_Envelope();
	}

	double intMinX = minX > env.minX ? minX : env.minX;
	double intMinY = minY > env.minY ? minY : env.minY;
	double intMaxX = maxX < env.maxX ? maxX : env.maxX;
	double intMaxY = maxY < env.maxY ? maxY : env.maxY;
	CG_Envelope env1;
	env1.maxX = intMaxX;
	env1.maxY = intMaxY;
	env1.minX = intMinX;
	env1.minY = intMinY;
	return env1;
}

//锟斤拷锟姐到锟斤拷一锟斤拷MBR锟侥撅拷锟斤拷
double CG_Envelope::DistanceTo(CG_Envelope &env)
{
	//锟斤拷锟斤拷嘟伙拷锟斤拷锟斤拷锟斤拷锟轿�
	if (InterSects(env))
	{
		return 0;
	}
	double dx = 0;
	if (maxX < env.minX)
	{
		dx = env.minX - maxX;
	}
	if (minX > env.maxX)
	{
		dx = minX - env.maxX;
	}

	double dy = 0;
	if (maxY < env.minY)
	{
		dy = env.minY - maxY;
	}
	if (minY > env.maxY)
	{
		dy = minY - env.maxY;
	}

	//锟斤拷锟斤拷锟斤拷锟街�晃�锟斤拷锟斤拷锟斤拷锟剿�斤拷锟斤拷叽锟街憋拷锟斤拷锟�
	if (0.0 == dx)
	{
		return dy;
	}
	if (0.0 == dy)
	{
		return dx;
	}
	return sqrt(dx*dx + dy*dy);
}

//锟斤拷锟斤拷锟斤拷锟�
double CG_Envelope::Area() const
{
	if (IsNull())
	{
		return 0;
	}
	return GetHeight()*GetWidth();
}

//锟斤拷锟斤拷锟杰筹拷
double CG_Envelope::Perimeter() const
{
	if (IsNull())
	{
		return 0;
	}
	return GetWidth()*2 + GetHeight()*2;
}

//锟斤拷锟斤拷锟角凤拷锟�
bool CG_Envelope::Contains(double x, double y) const
{
	if (IsNull()) 
		return false;

	return x >= minX &&
		x <= maxX &&
		y >= minY &&
		y <= maxY;
}

//锟叫讹拷p1,p2锟斤拷锟缴的撅拷锟轿猴拷q1,q2锟斤拷锟缴的撅拷锟斤拷锟角凤拷锟洁交
bool CG_Envelope::Intersects(const SPOINT &p1, const SPOINT &p2, const SPOINT &q1, const SPOINT &q2)
{
	double minq = min(q1.x, q2.x);
	double maxq = max(q1.x, q2.x);
	double minp = min(p1.x, p2.x);
	double maxp = max(p1.x, p2.x);

	if( minp > maxq )
		return false;
	if( maxp < minq )
		return false;

	minq = min(q1.y, q2.y);
	maxq = max(q1.y, q2.y);
	minp = min(p1.y, p2.y);
	maxp = max(p1.y, p2.y);

	if( minp > maxq )
		return false;
	if( maxp < minq )
		return false;
	return true;
}

bool CG_Envelope::Intersects(const CG_Envelope &env1, const CG_Envelope &env2)
{
	SPOINT p1(env1.minX,env1.minY);
	SPOINT p2(env1.maxX,env1.maxY); 
	SPOINT q1(env2.minX,env2.minY);
	SPOINT q2(env2.maxX,env2.maxY);
	return Intersects(p1,p2,q1,q2);
}
