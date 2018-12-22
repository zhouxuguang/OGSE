#include "GeoMercatorProjection.h"
#include "GeoEllipsoid.h"



GeoMercatorProjection::GeoMercatorProjection(const GeoEllipsoid& ellipsoid,
										   double dbOriginLat ,double dbOriginLon):
								GeoMapProjection(ellipsoid ,dbOriginLat , dbOriginLon )
{
	//设置投影参数
	double dMerc_Origin_Lat = dbOriginLat * DEG_TO_RAD;
	double dMerc_Origin_Long = dbOriginLon * DEG_TO_RAD;

	//设置椭球参数
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();

	SetMercatorParameters(dbA,dbB,dMerc_Origin_Lat,dMerc_Origin_Long,0,0);
}

GeoMercatorProjection::GeoMercatorProjection(const GeoEllipsoid& ellipsoid, 
										   const double dbOriginLat, 
										   const double dbOriginLon, 
										   const double dFalseEasting, 
										   const double dFalseNorthing)
										   :GeoMapProjection(ellipsoid,dbOriginLat,dbOriginLon)
{
	//设置投影参数
	double dMerc_Origin_Lat = dbOriginLat * DEG_TO_RAD;
	double dMerc_Origin_Long = dbOriginLon * DEG_TO_RAD;

	//设置椭球参数
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();

	SetMercatorParameters(dbA,dbB,dMerc_Origin_Lat,dMerc_Origin_Long,dFalseEasting,dFalseNorthing);
}

GeoMercatorProjection::~GeoMercatorProjection(void)
{
}

void GeoMercatorProjection::SetMercatorParameters(double a, double b, 
												 double Origin_Latitude, double Central_Meridian, 
												 double False_Easting, double False_Northing)
{
	//设置投影参数
	m_dMerc_False_Easting = False_Easting;
	m_dMerc_False_Northing = False_Northing;
	m_dMerc_Origin_Lat = Origin_Latitude;
	m_dMerc_Origin_Long = Central_Meridian;

	//设置椭球参数
	m_dMerc_a = a;
	m_dMerc_f = (a - b)/a;
	m_dMerc_e1 = sqrt(a*a - b*b) / a;
	m_dMerc_e2 = sqrt(a*a - b*b) / b;
}

bool GeoMercatorProjection::ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	//先将经纬度化为弧度
	dbLat = dbLat * DEG_TO_RAD;
	double dbDetLon = dbLon * DEG_TO_RAD;
	dbDetLon -= m_dMerc_Origin_Long;

	//计算等量纬度
	double dbTemp = (1.0-m_dMerc_e1*sin(dbLat))/
		(1.0+m_dMerc_e1*sin(dbLat));
	double dbExp = m_dMerc_e1/2.0;
	double dbEquard = pow(dbTemp , dbExp);
	double dbU = tan(M_PI_4 + dbLat/2.0) * dbEquard;

	double dbmMerc_b = sqrt(m_dMerc_a * m_dMerc_a - m_dMerc_e1 * m_dMerc_a);

	//计算系数K
	double dbDenominator = dbmMerc_b * sqrt(1.0 + 
		m_dMerc_e2 * m_dMerc_e2 * cos(m_dMerc_Origin_Lat) * cos(m_dMerc_Origin_Lat));
	double K =  m_dMerc_a * m_dMerc_a * cos(m_dMerc_Origin_Lat) / dbDenominator;

	//计算X和y
	dbNorth = log(dbU)*K + m_dMerc_False_Northing;
	dbEast = K*dbDetLon + m_dMerc_False_Easting;

	return true;
}

bool GeoMercatorProjection::InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const
{
	double dbmMerc_b = sqrt(m_dMerc_a * m_dMerc_a - m_dMerc_e1 * m_dMerc_a);

	//计算系数K
	double dbDenominator = dbmMerc_b * sqrt(1.0 + 
		m_dMerc_e2 * m_dMerc_e2 * cos(m_dMerc_Origin_Lat) * cos(m_dMerc_Origin_Lat));
	double K =  m_dMerc_a * m_dMerc_a * cos(m_dMerc_Origin_Lat) / dbDenominator;

	//计算经度
	dbLon = ((dbEast - m_dMerc_False_Easting)/K) + m_dMerc_Origin_Long;

	////计算t
	double t = exp((m_dMerc_Origin_Long - dbNorth) / K);

	//计算x0
	double x0 = M_PI_2 - 2*atan(t);

	//计算系数ABCD
	double A1 = 0.5*pow(m_dMerc_e1,2.0) + (5.0/24.0)*pow(m_dMerc_e1,4.0)
		+ 1.0/12.0*pow(m_dMerc_e1,6.0) + 13.0/360.0*pow(m_dMerc_e1,8.0);
	double B1 = 7.0/48.0*pow(m_dMerc_e1,4.0) + 29.0/240.0*pow(m_dMerc_e1,6.0)
		+ 811.0/11520.0*pow(m_dMerc_e1,8.0);
	double C1 = 7.0/120.0*pow(m_dMerc_e1,6.0) + 81.0/1120.0*pow(m_dMerc_e1,8.0);
	double D1 = 4279.0/161280.0*pow(m_dMerc_e1,8.0);

	dbLat = x0 + A1*sin(2*x0) + B1*sin(4*x0) + C1*sin(6*x0) + D1*sin(6*x0);

	dbLon *= RAD_TO_DEG;
	dbLat *= RAD_TO_DEG;

	return true;
}
