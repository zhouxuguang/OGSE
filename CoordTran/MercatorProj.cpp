#include "MercatorProj.h"

CMercatorProj::CMercatorProj(CGeoEllipse* pGeoEllipse,double dbCenterLon,double dbStdLat,
							 double dbFalseE,double dbFalseN)
{
	m_geoEllipse = pGeoEllipse;
	m_dbCenterLon = dbCenterLon;
	m_dbStdLat = dbStdLat;
	m_dbFalseE = dbFalseE;
	m_dbFalseN = dbFalseN;
	m_dbFalseE = 0;
	m_dbFalseN = 0;
}

CMercatorProj::~CMercatorProj(void)
{
}

void CMercatorProj::LonLat2Mercator(double dbLat,double dbLon,double &x,double &y)
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	//先将经纬度化为弧度
	dbLat = dbLat*M_PI/180.0;
	double dbDetLon = (dbLon-m_dbCenterLon)*M_PI/180.0;

	//计算等量纬度
	double dbU = m_geoEllipse->GetValueU(dbLat);

	//计算系数K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//计算X和y
	y = log(dbU)*K + m_dbFalseN;
	x = K*dbDetLon + m_dbFalseE;
}

void CMercatorProj::Mercator2LonLat(double x,double y,double &dbLat,double &dbLon)
{
	//计算系数K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//计算经度
	dbLon = ((x-m_dbFalseE)/K)*180.0/M_PI + m_dbCenterLon;

	//计算t
	double t = exp((m_dbFalseN-y) / K);

	//计算x0
	double x0 = M_PI_2 - 2*atan(t);

	//计算系数ABCD
	double A1 = 0.5*pow(m_geoEllipse->GetE1(),2.0) + (5.0/24.0)*pow(m_geoEllipse->GetE1(),4.0)
		+ 1.0/12.0*pow(m_geoEllipse->GetE1(),6.0) + 13.0/360.0*pow(m_geoEllipse->GetE1(),8.0);
	double B1 = 7.0/48.0*pow(m_geoEllipse->GetE1(),4.0) + 29.0/240.0*pow(m_geoEllipse->GetE1(),6.0)
		+ 811.0/11520.0*pow(m_geoEllipse->GetE1(),8.0);
	double C1 = 7.0/120.0*pow(m_geoEllipse->GetE1(),6.0) + 81.0/1120.0*pow(m_geoEllipse->GetE1(),8.0);
	double D1 = 4279.0/161280.0*pow(m_geoEllipse->GetE1(),8.0);

	dbLat = x0 + A1*sin(2*x0) + B1*sin(4*x0) + C1*sin(6*x0) + D1*sin(6*x0);
	dbLat = dbLat*180.0/M_PI;

}

void CMercatorProj::Mercator2LonLat2(double x,double y,double &dbLat,double &dbLon)
{
	//计算系数K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//计算经度
	dbLon = ((x-m_dbFalseE)/K)*180.0/M_PI + m_dbCenterLon;

	//使用迭代法求解纬度
	double B0 = 1,B1 = M_PI_2;		//纬度初始值设定
	int nCount = 200;				//迭代次数
	double eps = 1.0e-9;			//精度
	for (int i = 0; i < nCount; i ++)
	{
		//迭代式
		B1 = M_PI_2 - 2*atan(exp(-y/K) * 
			pow((1.0-m_geoEllipse->GetE1()*sin(B0))/(1.0+m_geoEllipse->GetE1()*sin(B0)),
			m_geoEllipse->GetE1()/2.0));
		if (fabs(B1-B0) < eps)
		{
			break;
		}
		else
		{
			B0 = B1;
		}
	}

	dbLat = B1*180.0/M_PI;
}