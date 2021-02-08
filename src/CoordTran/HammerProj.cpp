#include "HammerProj.h"
#include "GeoEllipse.h"

HammerProj::HammerProj(CGeoEllipse* pGeoEllipse,double dbCenterLon)
{
	m_geoEllipse = pGeoEllipse;
	m_dbCenterLon = dbCenterLon;
}

HammerProj::~HammerProj(void)
{
}

void HammerProj::LatLon2Hammer(double dbLat,double dbLon,double &x,double &y)
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	dbLon -= m_dbCenterLon;

	//将一些角度值转换为弧度制
	dbLat = dbLat * DEG_TO_RAD;
	dbLon = dbLon * DEG_TO_RAD;

	double dbSinLat = sin(dbLat);
	double dbCosLat = cos(dbLat);
	double dbSinLon2 = sin(dbLon/2);
	double dbCosLon2 = cos(dbLon/2);

	y = sqrt(2.0)*dbSinLat / 
		(sqrt(1 + dbCosLat * dbCosLon2));

	x = 2*sqrt(2.0)*dbCosLat*dbSinLon2 / 
		(sqrt(1 + dbCosLat * dbCosLon2));
}

void HammerProj::Hammer2LatLon(double x,double y,double &dbLat,double &dbLon)
{
	//x /= m_geoEllipse->GetAveRadius();
	//y /= m_geoEllipse->GetAveRadius();
	double dbZ = sqrt(1.0-x*x/16-y*y*0.25);
	dbLon = 2*atan2(dbZ*x,2*(2*dbZ*dbZ-1));
	dbLon *= RAD_TO_DEG;
	dbLon += m_dbCenterLon;
	dbLat = asin(dbZ*y);

	dbLat *= RAD_TO_DEG;
}
