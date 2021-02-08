#include "GeoEllipse.h"
#include "GaussProj.h"

CGaussProj::CGaussProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
					   double dbFalseN,double dbFalseE,int nType)
{
	m_pGeoEllipse = pGeoEllipse;
	m_dbCenterLat = dbCenterLat;
	m_dbCenterLon = dbCenterLon;
	m_dbFalseE = dbFalseE;
	m_dbFalseN = dbFalseN;
	m_nType = nType;
}

CGaussProj::~CGaussProj(void)
{
}

bool CGaussProj::LonLat2Gauss(int zoneWide,double dbLat,double dbLon,double *x,double *y)
{
	//转化为弧度
	double B = dbLat*M_PI/180;
	double L = dbLon*M_PI/180;
	double l = (dbLon-m_dbCenterLon)*M_PI/180;
	double dbE1 = m_pGeoEllipse->GetE1();
	double X = 0;//从赤道起算的子午线弧长

	//计算子午线弧长的系数
	double A0 = 1.0+3.0/4.0*pow(dbE1, 2)+45.0/64.0*pow(dbE1, 4)
		+175.0/256.0*pow(dbE1, 6)+11025.0/16384.0*pow(dbE1, 8);
	double A2 = 3.0/8.0*pow(dbE1, 2)+15.0/32.0*pow(dbE1, 4)+
		525.0/1024.0*pow(dbE1, 6)+2205.0/4096.0*pow(dbE1, 8);
	double A4 = 15.0/256.0*pow(dbE1, 4)+105.0/1024.0*pow(dbE1, 6)+
		2205.0/16384.0*pow(dbE1, 8);
	double A6 = 35.0/3072.0*pow(dbE1, 6)+105.0/4096.0*pow(dbE1, 8);
	double A8 = 315.0/131072.0*pow(dbE1, 8);
	//计算子午线弧长X
	X = m_pGeoEllipse->GetRadiusA()*(1.0-pow(dbE1, 2))*(A0*B - A2*sin(2*B) + A4*sin(4*B)
		- A6*sin(6*B) + A8*sin(8*B));
	double t = tan(B);
	double anke = cos(B)*sqrt(pow(dbE1,2.0)/(1.0-pow(dbE1,2.0)));	//?
	double N = m_pGeoEllipse->GetRadiusA()/sqrt(1.0-pow(dbE1, 2.0)*pow(sin(B), 2.0));
	//坐标计算

	*y = X + N*sin(B)*cos(B)*pow(l, 2)/2.0+
		N*t*pow(cos(B), 4)*pow(l, 4)*(5.0-pow(t, 2)+9*pow(anke, 2)+4*pow(anke, 4))/24.0
		+ N*t*pow(l*cos(B), 6)*(61.0-58*pow(t, 2)+pow(t, 4)+270.0*pow(anke, 2)-330.0*pow(anke, 2)*pow(t, 2))/720.0;

	*x = N*cos(B)*l + 1.0/6*N*(1.0-pow(t, 2)+pow(anke, 2))*pow(cos(B), 3)*pow(l, 3)+
		1.0/120*N*(5.0-18*t*t+pow(t, 4)+14*pow(anke, 2)-58*anke*anke*t*t)*
		pow(cos(B), 3)*pow(l, 5);
	*x += m_dbFalseE;

	return 1;
}

bool CGaussProj::Gauss2LonLat(double dbX,double dbY,double *dbLat,double *dbLon)
{
	double x = dbX - m_dbFalseE;
	double y = dbY - m_dbFalseN;

	double dbA = m_pGeoEllipse->GetRadiusA();
	double dbE1 = m_pGeoEllipse->GetE1();
	double dbE2 = m_pGeoEllipse->GetE2();

	//首先计算A1
	double A1 = dbA*(1.0-pow(dbE1,2.0))*(1.0+3.0/4.0*pow(dbE1, 2)+45.0/64.0*pow(dbE1, 4)
		+175.0/256.0*pow(dbE1, 6)+11025.0/16384.0*pow(dbE1, 8));
	double A2 = dbA*(1.0-pow(dbE1,2.0))*(3.0/4.0*pow(dbE1, 2)+45.0/64.0*pow(dbE1, 4)+
		175.0/256.0*pow(dbE1, 6)+11025.0/16384.0*pow(dbE1, 8));
	double A3 = dbA*(1.0-pow(dbE1,2.0))*(15.0/32.0*pow(dbE1, 4)+175.0/384.0*pow(dbE1, 6)+
		3675.0/8192.0*pow(dbE1, 8));
	double A4 = dbA*(1.0-pow(dbE1,2.0))*(35.0/96.0*pow(dbE1, 6)+735.0/2048.0*pow(dbE1, 8));
	double A5 = dbA*(1.0-pow(dbE1,2.0))*(315.0/1024.0*pow(dbE1, 8));

	//初始化一些值
	double B0 = y/A1;
	double a1 = dbA*cos(B0)/sqrt(1.0-pow(dbE1*sin(B0),2));
	double l0 = x/a1;

	double N,a2,a3,a4,a5,a6;

	double eps = pow(10.0,-10.0);   //定义容差
	double tempB = B0;
	double tempL = l0;
	double epsB,epsL;	//经纬度的容差
	int count = 0;	//迭代次数
	do 
	{
		N = dbA/sqrt(1.0-pow(dbE1*sin(B0),2.0));

		//计算纬度
		a2 = N*sin(B0)*cos(B0)/2;
		a4 = N*sin(B0)*pow(cos(B0),3)*(5.0-pow(tan(B0),2.0)
			+9*pow(dbE2*cos(B0),2)+4*pow(dbE2*cos(B0),4))/24;
		a6 = N*sin(B0)*pow(cos(B0),5)*(61.0-58*pow(tan(B0),2)+pow(tan(B0),4))/720;
		//tempB = (y-(a3*pow(l0,3.0)+a5*pow(l0,5.0)))/A1;
		tempB = (y-(A2-(A3-(A4-A5*pow(sin(B0),2)*pow(sin(B0),2)))*pow(sin(B0),2))*sin(B0)*cos(B0)-
			a2*pow(l0,2.0)+a4*pow(l0,4.0)+a6*pow(l0,6.0))/A1;
		a1 = N*cos(tempB);

		//计算经度
		a3 = N*pow(cos(tempB),3.0)*(1.0-pow(tan(tempB),2.0)+pow(dbE2*cos(tempB),2.0))/6.0;
		a5 = N*pow(cos(tempB),5)*(5.0-18*pow(tan(tempB),2)+pow(tan(tempB),4)+
			14*pow(dbE2*cos(tempB),2)-58*pow(dbE2*sin(tempB),2))/120;
		tempL = (x-(a3*pow(l0,3.0)+a5*pow(l0,5.0)))/A1;

		epsB = fabs(tempB-B0);
		epsL = fabs(tempL-l0);

		//更新B0和l0的值
		B0 = tempB;
		l0 = tempL;
		count ++;
	} while ((epsB>eps) && (epsL>eps));

	*dbLat = tempB*180.0/M_PI;
	*dbLon = tempL*180.0/M_PI;
	*dbLon = *dbLon + m_dbCenterLon;//中央经线+经度差
	return 1;
}
