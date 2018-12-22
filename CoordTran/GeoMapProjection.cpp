#include "GeoMapProjection.h"


GeoMapProjection::~GeoMapProjection(void)
{
}

GeoMapProjection::GeoMapProjection(const GeoEllipsoid& ellipsoid , 
								   double dbOriginLat /*= 0*/,double dbOriginLon /*= 0*/)
{

}

GeoMapProjection::GeoMapProjection(const GeoMapProjection& src)
{

}

void GeoMapProjection::GetOrigin(double &dbOriginLat,double &dbOriginLon) const
{
	dbOriginLon = m_dbOriginLon;
	dbOriginLat = m_dbOriginLat;
}

double GeoMapProjection::GetFalseEasting() const
{
	//return m_dbFalseEasting;
	return 0;
}

double GeoMapProjection::GetFalseNorthing() const
{
	//return m_dbFalseNorthing;
	return 0;
}

double GeoMapProjection::GetStandardParallel1() const
{
	return 0;
}

double GeoMapProjection::GetStandardParallel2() const
{
	return 0;
}

void GeoMapProjection::Update()
{
	return;
}

std::string GeoMapProjection::GetProjectionName() const
{
	return "";
}

double GeoMapProjection::GetA() const
{
	return m_dbA;
}

double GeoMapProjection::GetB() const
{
	return m_dbB;
}

double GeoMapProjection::GetF() const
{
	return m_dbF;
}

const GeoEllipsoid& GeoMapProjection::GetEllipsoid() const
{
	return m_Ellipsoid;
}

bool GeoMapProjection::IsGeographic() const
{
	return false;
}

void GeoMapProjection::SetEllipsoid(const GeoEllipsoid& ellipsoid)
{
	m_Ellipsoid = ellipsoid;
}

void GeoMapProjection::SetAB(double a, double b)
{
	m_dbA = a;
	m_dbB = b;
}
