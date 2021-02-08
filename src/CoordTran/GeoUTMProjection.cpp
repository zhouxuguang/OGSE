#include "GeoUTMProjection.h"
#include "GeoTranmerc.h"


GeoUTMProjection::~GeoUTMProjection(void)
{
}

GeoUTMProjection::GeoUTMProjection(const GeoEllipsoid& ellipsoid, 
								   const double dbOriginLat, const double dbOriginLon, 
								   int nZone, char cHemisphere, 
								   const double dFalseEasting, const double dFalseNorthing)
								   :GeoMapProjection(ellipsoid)
{
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();
	double dbLat = dbOriginLat * DEG_TO_RAD;
	double dbLon = dbOriginLon * DEG_TO_RAD;

	SetMercatorParameters(dbA,dbB,dbLat,dbLon,dFalseEasting,dFalseNorthing,nZone,cHemisphere);
	
}

void GeoUTMProjection::SetMercatorParameters(double a, double b, 
											 double Origin_Latiude, double Central_Meridian, 
											 double False_Easting, double False_Northing, 
											 int nZone, char cHemisphere)
{
	m_dbTranMerc_a = a;
	m_dbTranMerc_b = b;
	m_dbTranMerc_f = (a - b) / a;
	m_dbTranMerc_e = sqrt(a*a - b*b)/a;
	m_dbTranMerc_Origin_Lat = Origin_Latiude;
	m_dbTranMerc_Origin_Long = Central_Meridian;
	m_dbTranMerc_Scale_Factor = 0.9996;
	m_nZone = nZone;
	m_cHemisphere = cHemisphere;
	m_dbTranMerc_False_Easting = False_Easting;
	m_dbTranMerc_False_Northing = False_Northing;
}

bool GeoUTMProjection::ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const
{
	GeodeicalToMercator(m_dbTranMerc_a,m_dbTranMerc_b,dbLat,dbLon,m_dbTranMerc_Origin_Long,dbEast,dbNorth);

	dbEast = dbEast * m_dbTranMerc_Scale_Factor + m_dbTranMerc_False_Easting;
	dbNorth = dbNorth * m_dbTranMerc_Scale_Factor + m_dbTranMerc_False_Northing;

	/*if (northing < 0.0)
	{
		northing += 10000000.0;
		isNorth = false;
	}*/

	return true;
}

bool GeoUTMProjection::InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const
{
	//先将UTM坐标转换为高斯平面坐标
	dbNorth -= m_dbTranMerc_False_Northing;
	dbEast -= m_dbTranMerc_False_Easting;

	dbEast /= m_dbTranMerc_Scale_Factor;
	dbNorth /= m_dbTranMerc_Scale_Factor;
	MercatorToGeodeical(m_dbTranMerc_a,m_dbTranMerc_b,dbEast,dbNorth,m_dbTranMerc_Origin_Long,dbLat,dbLon);
	return true;
}
