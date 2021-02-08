
#include <math.h>
#include "GeoEllipse.h"
#include "LambertProj.h"

CLambertProj::CLambertProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
						   double dbStdLat1,double dbStdLat2,
						   double dbFalseE,double dbFalseN)
{
	m_geoEllipse = pGeoEllipse;
	m_dbCenterLat = dbCenterLat;
	m_dbCenterLon = dbCenterLon;
	m_dbStdLat1 = dbStdLat1;
	m_dbStdLat2 = dbStdLat2;
	m_dbFalseE = dbFalseE;
	m_dbFalseN = dbFalseN;
}

CLambertProj::~CLambertProj(void)
{
}

void CLambertProj::LatLon2Lambert(double dbLat,double dbLon,double &x,double &y)
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	//将一些角度值转换为弧度制
	dbLat = dbLat * M_PI /180.0;
	dbLon = dbLon * M_PI /180.0;

	//经度差
	double dbDetLon = (dbLon-m_dbCenterLon*M_PI/180.0);
	
	//计算U值
	/*double dbU1 = GetValueU(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbU2 = GetValueU(m_geoEllipse,m_dbStdLat2*M_PI/180.0);
	double dbU = GetValueU(m_geoEllipse,dbLat);
	double dbU0 = GetValueU(m_geoEllipse,m_dbCenterLat*M_PI/180.0);*/

	//计算M值
	double dbM1 = GetValueM(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbM2 = GetValueM(m_geoEllipse,m_dbStdLat2*M_PI/180.0);

	//计算T值
	double dbT1 = GetValueT(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbT2 = GetValueT(m_geoEllipse,m_dbStdLat2*M_PI/180.0);
	double dbT0 = GetValueT(m_geoEllipse,m_dbCenterLat*M_PI/180.0);

	//计算N
	double dbN = (log(dbM1)-log(dbM2))/(log(dbT1)-log(dbT2));

	//计算F值
	double dbF = dbM1/(dbN*pow(dbT1,dbN));

	double r0 = m_geoEllipse->GetRadiusA()*dbF*pow(dbT0,dbN);

	//计算thea
	double dbThea = dbN * dbDetLon;

	double r = m_geoEllipse->GetRadiusA()*dbF*pow(GetValueT(m_geoEllipse,dbLat),dbN); 
	
	//x = m_geoEllipse->GetRadiusA()*dbF*pow(GetValueT(m_geoEllipse,dbLat),dbN)*sin(dbThea) + m_dbFalseE;
	x = r*sin(dbThea) + m_dbFalseE;
	y = r0 - r*cos(dbThea) + m_dbFalseN;
}

void CLambertProj::Lambert2LatLon(double x,double y,double &dbLat,double &dbLon)
{
	//计算U值
	double dbU1 = GetValueU(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbU2 = GetValueU(m_geoEllipse,m_dbStdLat2*M_PI/180.0);
	//double dbU = GetValueU(m_geoEllipse,dbLat);
	double dbU0 = GetValueU(m_geoEllipse,m_dbCenterLat*M_PI/180.0);

	//计算两条标准纬线的的半径
	double r1 = m_geoEllipse->GetLatCycleRadius(m_dbStdLat1);
	double r2 = m_geoEllipse->GetLatCycleRadius(m_dbStdLat2);

	//计算常数σ
	double dbMiu = (log(r2)-log(r1))/(log(dbU1)-log(dbU2));

	//计算常数K
	double K = r1*pow(dbU1,dbMiu)/dbMiu;

	//计算m的值
	double dbM1 = GetValueM(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbM2 = GetValueM(m_geoEllipse,m_dbStdLat2*M_PI/180.0);

	//计算T值
	double dbT1 = GetValueT(m_geoEllipse,m_dbStdLat1*M_PI/180.0);
	double dbT2 = GetValueT(m_geoEllipse,m_dbStdLat2*M_PI/180.0);
	double dbT0 = GetValueT(m_geoEllipse,m_dbCenterLat*M_PI/180.0);

	//计算N
	double dbN = (log(dbM1)-log(dbM2))/(log(dbT1)-log(dbT2));

	//计算F值
	double dbF = dbM1/(dbN*pow(dbT1,dbN));

	double r0 = m_geoEllipse->GetRadiusA()*dbF*pow(dbT0,dbN);
	//double r0 = m_geoEllipse->GetLatCycleRadius(m_dbCenterLat);

	//计算thea
	double dbThea = dbN * (dbLon-m_dbCenterLon)*M_PI/180.0;

	//计算投影原点的投影半径
	double dbRou0 = K/pow(dbU0,dbMiu);

	//计算r1
	double dbRou1 = Sign(dbN)*sqrt(x*x + (dbRou0-y)*(dbRou0-y));


	//计算t1的值
	double t1 = pow(dbRou1/(m_geoEllipse->GetRadiusA()*dbF),1.0/dbN);

	double B0 = 1,B1 = M_PI_2;	//纬度进行迭代计算
	double eps = 0.000000000000001;	//精度
	for (int i = 0; i < 1000; i ++)
	{
		B1 = M_PI_2 - 2* atan(t1 * pow((1.0-
			m_geoEllipse->GetE1()*sin(B0))/(1.0+m_geoEllipse->GetE1()*sin(B0)),m_geoEllipse->GetE1()/2.0));
		if (fabs(B1-B0)>eps)
		{
			B0 = B1;
		}
		else
			break;
		
	}

	dbLat = B1*180.0/M_PI;

	//计算经度
	double thea = atan((x-m_dbFalseE)/(r0-y+m_dbFalseN));
	dbLon = (thea/dbN)*180.0/M_PI + m_dbCenterLon;
	
}

double GetValueU(CGeoEllipse* pGeoEllipse,double dbLat)
{
	//计算U值
	double dbTemp = (1.0-pGeoEllipse->GetE1()*sin(dbLat))/
		(1.0+pGeoEllipse->GetE1()*sin(dbLat));
	double dbExp = pGeoEllipse->GetE1()/2.0;
	double dbEquard = pow(dbTemp , dbExp);
	double dbU =  tan(M_PI_4 + dbLat/2.0) * dbEquard;
	return dbU;
}

int Sign(double dbN)
{
	if (dbN < 0)
	{
		return -1;
	}
	else
		return 1;
}

double GetValueM(CGeoEllipse* pGeoEllipse,double dbLat)
{
	return cos(dbLat)/sqrt(1.0-
		pGeoEllipse->GetE1()*pGeoEllipse->GetE1()*sin(dbLat)*sin(dbLat));
}

double GetValueT(CGeoEllipse* pGeoEllipse,double dbLat)
{
	//double dbValue = 0.0;
	return tan(M_PI_4 - dbLat/2.0)/
		pow((1.0-pGeoEllipse->GetE1()*sin(dbLat)) 
		/ (1.0+pGeoEllipse->GetE1()*sin(dbLat)),pGeoEllipse->GetE1()/2.0);
}
