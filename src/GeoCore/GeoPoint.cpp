
#include <math.h>
#include <assert.h>
#include <sstream>
#include <limits>
#include "GeoEnvelope.h"
#include "GeoCoordinate.h"
#include "GeoPoint.h"


//Ĭ�ϵĹ��캯��
GeoPoint::GeoPoint(void)
{
	this->x = 0.0;
	this->y = 0.0;
}

//�������Ĺ��캯��
GeoPoint::GeoPoint(double x,double y)
{
	this->x = x;
	this->y = y;
}

//�������캯��
GeoPoint::GeoPoint(const GeoPoint& point)
{
	this->x = point.x;
	this->y = point.y;
}

GeoPoint::GeoPoint(GeoCoordinate *coord)
{
	this->x = coord->x;
	this->y = coord->y;
}

GeoPoint::~GeoPoint(void)
{
}

GeoPoint& GeoPoint::operator =(const GeoPoint &pt)
{
	this->x = pt.x;
	this->y = pt.y;

	return *this;
}

//��ռ�����
bool GeoPoint::Empty()
{
	this->x = 0.0;
	this->y = 0.0;
	return true;
}

//��õ��ά��
int GeoPoint::GetDimension() const
{
	//���ά����0
	return 0;
}

GeoWkbGeometryType GeoPoint::GetGeometryType() const
{
	return gWkbPoint;
}

const char *GeoPoint::GetGeometryName() const
{
	return "POINT";
}

void GeoPoint::GetEnvelope(GeoEnvelope *env) const
{
	if (NULL == env)
	{
		return;
	}
	env->minX = env->maxX = GetX();
	env->minY = env->maxY = GetY();
}

double GeoPoint::GetArea() const
{
	return (double)0;
}

double GeoPoint::GetLength() const
{
	return (double)0;
}

std::string GeoPoint::AsText()
{
	//�����ʾΪOGC��WKT��ʽ
	std::stringstream ss;
	int precise = std::numeric_limits<double>::digits10;
	ss.precision(precise);
	ss<<"POINT("<<x<<" "<<y<<")";
	return ss.str();
}

int GeoPoint::WkbByteSize() const
{
	return 21;
}

int GeoPoint::AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const
{
	poData[0] = (unsigned char) eByteOrder;

	unsigned int nGType = GetGeometryType();

	/*if( eByteOrder == wkbNDR )
		nGType = CPL_LSBWORD32( nGType );
	else
		nGType = CPL_MSBWORD32( nGType );*/

	//copy����������
	memcpy( poData + 1, &nGType, 4 );

	//copy����
	memcpy( poData+5, &x, 16 );

	if( m_nCoordDimension == 3 )
	{
		memcpy( poData + 5 + 16, &y, 8 );
	}
	
	return 1;
}

GeoGeometry* GeoPoint::Clone() const
{
	GeoPoint* poNewPoint = new GeoPoint(x,y);

	poNewPoint->SetSpatialReference( GetSpatialReference() );

	return poNewPoint;
}

double GeoPoint::GetX() const
{
	return this->x;
}

double GeoPoint::GetY() const
{
	return this->y;
}

void GeoPoint::SetX(double xIn)
{
	this->x = xIn;
}

void GeoPoint::SetY(double yIn)
{
	this->y = yIn;
}

//����һ����ľ���
double GeoPoint::DistanceTo(const GeoPoint &geoPoint)
{
	double dx = this->x - geoPoint.x;
	double dy = this->y - geoPoint.y;
	return sqrt(dx*dx + dy*dy);
}

//���Ƿ����߶���
bool GeoPoint::IsPointInLine(GeoPoint &p1, GeoPoint &p2)
{
	//�жϵ��Ƿ��ڱ߽������
	if (x < min(p1.GetX(),p2.GetX())|| x > max(p1.GetX(),p2.GetX())
		|| y < min(p1.GetY(),p2.GetY()) || y > max(p1.GetY(),p2.GetY()))
	{
		return false;
	}
	double dblMulti = 0.0;
	//������
	dblMulti = (double)((x-p1.GetX())*(p2.GetY()-p1.GetY())-
		(p2.GetX()-p1.GetX())*(y-p1.GetY()));
	if (0.0 == dblMulti)
	{
		return true;
	}
	else
		return false;
}

int GeoPoint::IsPointInRect(GeoEnvelope env)
{
	//�жϵ��Ƿ��ڱ߽������
	if (x >= env.minX && x <= env.maxX
		&& y >= env.minY && y <= env.maxY)
	{
		return true;
	}

	return false;
}

bool GeoPoint::IsEqual(GeoPoint &point)
{
	//�������֮��ľ���С���ݲ�����Ϊ����ȵ�
	if (fabs(x - point.x) < eps && fabs(y - point.y) < eps)
	{
		return true;
	}
	return false;
}

void GeoPoint::Move(double dx,double dy)
{
	x += dx;
	y += dy;
}
