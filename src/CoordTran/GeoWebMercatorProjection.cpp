#include "GeoWebMercatorProjection.h"



GeoWebMercatorProjection::GeoWebMercatorProjection(const GeoEllipsoid& ellipsoid, 
												   const double dbOriginLat /*= 0*/, 
												   const double dbOriginLon /*= 0*/, 
												   const double dFalseEasting /*= 0*/, 
												   const double dFalseNorthing /*= 0*/)
												   :GeoMapProjection(ellipsoid)
{
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();
	
	double dbOriginLat1 = dbOriginLat * DEG_TO_RAD;
	double dbOriginLon1 = dbOriginLon * DEG_TO_RAD;

	SetMercatorParameters(dbA,dbB,dbOriginLat1,dbOriginLon1,dFalseEasting,dFalseNorthing);
}

GeoWebMercatorProjection::~GeoWebMercatorProjection(void)
{
}

void GeoWebMercatorProjection::SetMercatorParameters(double a, double b, 
													 double Origin_Latiude, double Central_Meridian, 
													 double False_Easting, double False_Northing)
{
	m_dMercator_a = a;
	m_dMercator_e1 = sqrt(a * a - b * b) / a;
	m_dMercator_e2 = sqrt(a * a - b * b) / b;
	m_dMercator_f = (a - b) /a;

	m_dbFalseE = False_Easting;
	m_dbFalseN = False_Northing;
	m_dbCenterLat = Origin_Latiude;
	m_dbCenterLon = Central_Meridian;
}

bool GeoWebMercatorProjection::ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	dbLon *= DEG_TO_RAD;

	//先求得X坐标
	double dbDetLon = dbLon - m_dbCenterLon;
	dbEast = m_dbFalseE + m_dMercator_a*(dbDetLon);

	//下面求Y坐标
	dbLat *= DEG_TO_RAD;
	dbNorth = m_dbFalseN + m_dMercator_a*log(tan(M_PI_4 + dbLat/2.0));

	return true;
}

bool GeoWebMercatorProjection::InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const
{
	double D = (m_dbFalseN - dbNorth)/m_dMercator_a;
	dbLat = M_PI_2 - 2*atan(exp(D));
	dbLat *= RAD_TO_DEG;
	dbLon = (dbEast - m_dbFalseE)/m_dMercator_a + m_dbCenterLon;
	dbLon *= RAD_TO_DEG;

	return true;
}
