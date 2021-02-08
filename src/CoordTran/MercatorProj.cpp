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

	//�Ƚ���γ�Ȼ�Ϊ����
	dbLat = dbLat*M_PI/180.0;
	double dbDetLon = (dbLon-m_dbCenterLon)*M_PI/180.0;

	//�������γ��
	double dbU = m_geoEllipse->GetValueU(dbLat);

	//����ϵ��K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//����X��y
	y = log(dbU)*K + m_dbFalseN;
	x = K*dbDetLon + m_dbFalseE;
}

void CMercatorProj::Mercator2LonLat(double x,double y,double &dbLat,double &dbLon)
{
	//����ϵ��K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//���㾭��
	dbLon = ((x-m_dbFalseE)/K)*180.0/M_PI + m_dbCenterLon;

	//����t
	double t = exp((m_dbFalseN-y) / K);

	//����x0
	double x0 = M_PI_2 - 2*atan(t);

	//����ϵ��ABCD
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
	//����ϵ��K
	double K = m_geoEllipse->GetPrimeRadius(m_dbStdLat)*cos(m_dbStdLat*M_PI/180.0);

	//���㾭��
	dbLon = ((x-m_dbFalseE)/K)*180.0/M_PI + m_dbCenterLon;

	//ʹ�õ��������γ��
	double B0 = 1,B1 = M_PI_2;		//γ�ȳ�ʼֵ�趨
	int nCount = 200;				//��������
	double eps = 1.0e-9;			//����
	for (int i = 0; i < nCount; i ++)
	{
		//����ʽ
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