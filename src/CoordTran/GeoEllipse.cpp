#include "GeoEllipse.h"

CGeoEllipse::CGeoEllipse(double dbRadiusA,double dbRadiusB)
:m_dbRadiusA(dbRadiusA),m_dbRadiusB(dbRadiusB)
{
	//m_dbRadiusA = dbRadiusA;
	//m_dbRadiusB = dbRadiusB;

	//�����һ�͵ڶ�ƫ����
	m_dbE1 = sqrt(m_dbRadiusA*m_dbRadiusA - m_dbRadiusB*m_dbRadiusB) / m_dbRadiusA;
	m_dbE2 = sqrt(m_dbRadiusA*m_dbRadiusA - m_dbRadiusB*m_dbRadiusB) / m_dbRadiusB;
}

CGeoEllipse::~CGeoEllipse(void)
{
}

double CGeoEllipse::GetMeridianRadius(double lat) const
{
	//�Ƚ�γ�Ȼ���������
	lat = lat *  acos(-1.0)/180.0;
	double dbNumerator = m_dbRadiusA*(1.0-m_dbE1*m_dbE1); 
	double dbDenominator = pow(1.0 - m_dbE1*m_dbE1 * sin(lat)*sin(lat),1.5);
	return dbNumerator/dbDenominator;
}

double CGeoEllipse::GetPrimeRadius(double lat) const
{
	assert(lat >= -90 && lat <= 90);
	//�Ƚ�γ�Ȼ���������
	lat = lat * acos(-1.0)/180.0;
	
	double dbDenominator = sqrt(1.0-m_dbE1*m_dbE1 * sin(lat)*sin(lat));
	return m_dbRadiusA/dbDenominator;
}

double CGeoEllipse::GetLatCycleRadius(double lat) const
{
	assert(lat >= -90 && lat <= 90);
	double dbN = GetPrimeRadius(lat);
	lat = lat * acos(-1.0)/180.0;
	return dbN * cos(lat);
}

double CGeoEllipse::GetParallelArcLen(double dbLat,double dbDLon) const
{
	//������
	assert(dbLat >= -90 && dbLat <= 90
		&& dbDLon <= 180 && dbDLon >= 0);

	double dbN = GetPrimeRadius(dbLat);
	dbLat = dbLat * acos(-1.0)/180.0;
	dbDLon = dbDLon * acos(-1.0)/180.0;

	return dbN*cos(dbLat)*dbDLon;
}

double CGeoEllipse::GetMeridianArcLen(double dbLat) const
{
	//�������Ƿ�Ϸ�
	assert(dbLat >= -90 && dbLat <= 90);

	//���ȼ������ϵ����ֵ
	double dbA1 = 1.0 + 3.0/4.0*pow(m_dbE1,2.0) + 45.0/64.0*pow(m_dbE1,4.0)+
		175.0/256.0*pow(m_dbE1,6.0) + 11025.0/16384.0*pow(m_dbE1,8.0)
		+ 43659.0/65536.0*pow(m_dbE1,10.0) + 693693/1048576.0*pow(m_dbE1,12.0);

	double dbB1 = 3.0/4.0*pow(m_dbE1,2.0) + 45.0/64.0*pow(m_dbE1,4.0)+
		175.0/256.0*pow(m_dbE1,6.0) + 11025.0/16384.0*pow(m_dbE1,8.0)
		+ 43659.0/65536.0*pow(m_dbE1,10.0) + 693693/1048576.0*pow(m_dbE1,12.0);

	double dbC1 = 15.0/32.0*pow(m_dbE1,4.0) + 175.0/384.0*pow(m_dbE1,6.0) + 
		3675.0/8192.0*pow(m_dbE1,8.0) + 14553.0/32768.0*pow(m_dbE1,10.0) + 
		231231.0/524288.0*pow(m_dbE1,12.0);

	double dbD1 = 35.0/96.0*pow(m_dbE1,6.0) + 735.0/2048.0*pow(m_dbE1,8.0) + 
		14553.0/40960.0*pow(m_dbE1,10.0) + 231231.0/655360.0*pow(m_dbE1,12.0);

	double dbE1 = 315.0/1024.0*pow(m_dbE1,8.0) + 6237.0/20480.0*pow(m_dbE1,10.0) + 
		99099.0/327680.0*pow(m_dbE1,12.0);

	double dbF1 = 693.0/2560.0*pow(m_dbE1,10.0) + 11011.0/40960.0*pow(m_dbE1,12.0);

	double dbG1 = 1001.0/4096.0*pow(m_dbE1,12.0);

	//���Ƕ�ת��Ϊ����
	dbLat = /*fabs*/(dbLat * acos(-1.0)/180.0);

	return m_dbRadiusA*(1.0-pow(m_dbE1,2.0)) * 
		(dbA1*dbLat - cos(dbLat) * (dbB1*sin(dbLat) + dbC1*pow(sin(dbLat),3.0) + 
		dbD1*pow(sin(dbLat),5.0) + dbE1*pow(sin(dbLat),7.0) + dbF1*pow(sin(dbLat),9.0) + 
		dbG1*pow(sin(dbLat),11.0)));
}

double CGeoEllipse::GetTrapeziumArea(double dbLat1,double dbLat2,double dbLon1,double dbLon2) const
{
	//�������Ƿ�Ϸ�
	assert(dbLat1 >= -90 && dbLat1 <= 90
		&& dbLon1 <= 180 && dbLon1 >= -180);
	assert(dbLat2 >= -90 && dbLat2 <= 90
		&& dbLon2 <= 180 && dbLon2 >= -180);

	//������ص�ϵ��
	dbLon1 = dbLon1 * acos(-1.0)/180.0;
	dbLon2 = dbLon2 * acos(-1.0)/180.0;

	double dbK = 2 * m_dbRadiusA*m_dbRadiusA * (1.0-m_dbE1*m_dbE1) * (dbLon2-dbLon1);

	double dbA = 1.0 + 0.5*m_dbE1*m_dbE1 + 3.0/8.0*pow(m_dbE1,4.0) + 
		5.0/16.0*pow(m_dbE1,6.0) + 630.0/2304.0*pow(m_dbE1,8.0);

	double dbB = 1.0/6.0*m_dbE1*m_dbE1 + 0.3*pow(m_dbE1,4.0) + 
		3.0/16.0*pow(m_dbE1,6.0) + 420.0/2304.0*pow(m_dbE1,8.0);

	double dbC = 3.0/80.0*pow(m_dbE1,4.0) + 1.0/16.0*pow(m_dbE1,6.0) + 180.0/2304.0*pow(m_dbE1,8.0);

	double dbD = 1.0/112.0*pow(m_dbE1,6.0) + 45.0/2304.0*pow(m_dbE1,8.0);

	double dbE = 5.0/2304.0*pow(m_dbE1,8.0);

	//����γ�Ȳ�м�γ�ȵ�
	dbLat1 = dbLat1 * acos(-1.0)/180.0;
	dbLat2 = dbLat2 * acos(-1.0)/180.0;
	double dbDetLat = fabs(dbLat2-dbLat1);
	double dbMidLat = (dbLat1+dbLat2)/2.0;

	//������
	return dbK * (dbA*sin(dbDetLat/2.0)*cos(dbMidLat) - dbB*sin(3*dbDetLat/2.0)*cos(3*dbMidLat)
		+ dbC*sin(5*dbDetLat/2.0)*cos(5*dbMidLat) - dbD*sin(7*dbDetLat/2.0)*cos(7*dbMidLat) + 
		dbE*sin(9*dbDetLat/2.0)*cos(9*dbMidLat)) ;
}

double CGeoEllipse::GetValueU(double dbLat)
{
	double dbTemp = (1.0-m_dbE1*sin(dbLat))/
		(1.0+m_dbE1*sin(dbLat));
	double dbExp = m_dbE1/2.0;
	double dbEquard = pow(dbTemp , dbExp);
	return tan(M_PI_4 + dbLat/2.0) * dbEquard;
}

bool CGeoEllipse::BLH_XYZ(double dbLon,double dbLat,double dbHei,double& x,double &y,double &z)
{
	//�����Ϸ��Լ��
	assert(dbLat >= -90 && dbLat <= 90);
	assert(dbLon >= -180 && dbLon <= 180);

	//����î��Ȧ���ʰ뾶
	double dbN = GetPrimeRadius(dbLat);

	double B = dbLat * DEG_TO_RAD; //ת��Ϊ������
	double L = dbLon * DEG_TO_RAD; 
	double H = dbHei;
	
	//����ռ�ֱ������
	x = (dbN + H)*cos(B)*cos(L);
	y = (dbN + H)*cos(B)*sin(L);
	z = (dbN*(1.0-m_dbE1*m_dbE1)+ H)*sin(B);

	//ת���ɹ�
	return true;
}

bool CGeoEllipse::XYZ_BLH(double X,double Y,double Z, double &dbLon,double &dbLat,double &dbHei)
{
	//double dDet = 0.0000000001;		//��ֵ
//#ifdef 0
//	//��⾭��
//	dbLon = atan(Y/X);
//	if (X < 0)
//	{
//		dbLon += M_PI;
//	}
//
//	//���γ��
//	double r = /*pow(X,2.0)+pow(Y,2.0)*/X*X + Y*Y;
//	double R = Z*Z + r;
//
//	double dbU = atan(Z/sqrt(r)*sqrt(1.0-m_dbE1*m_dbE1));
//
//	double dbTemp = (Z + m_dbE2*m_dbE2*m_dbRadiusB*pow(sin(dbU),3.0) ) / 
//		(sqrt(r) - m_dbE1*m_dbE1*m_dbRadiusA*pow(cos(dbU),3.0));
//
//	dbLat = atan(dbTemp);
//
//	//���߳�
//	dbHei = sqrt(r)*cos(dbLat) + Z*sin(dbLat) - 
//		m_dbRadiusA*(sqrt(1.0-m_dbE1*m_dbE1*pow(sin(dbLat),2.0)));
//
//	//ת��Ϊ�Ƕ�ֵ
//	dbLon = (dbLon/acos(-1.0))*180;
//	dbLat = (dbLat/acos(-1.0))*180;
//
//	//ת���ɹ�
//	return true;
//
//#else
	//��⾭��
	dbLon = atan2(Y,X);
	/*if (X < 0)
	{
		dbLon += M_PI;
	}*/

	//���γ��
	double r = X*X + Y*Y;
	double R = Z*Z + r;

	//���һЩ�м����
	double dbPhi = atan(Z/sqrt(r));

	double dbU = ( m_dbRadiusB *Z * (1 + m_dbE2*m_dbE2*m_dbRadiusB/sqrt(R)) ) / (m_dbRadiusA*sqrt(r));
	dbU = atan(dbU);
	
	double dbTemp = (Z + m_dbE2*m_dbE2*m_dbRadiusB* sin(dbU) * sin(dbU) * sin(dbU) ) / 
				(sqrt(r) - m_dbE1*m_dbE1*m_dbRadiusA*cos(dbU) * cos(dbU) * cos(dbU));

	dbLat = atan(dbTemp);

	//���߳�
	dbHei = sqrt(r)*cos(dbLat) + Z*sin(dbLat) - 
			m_dbRadiusA*(sqrt(1.0-m_dbE1*m_dbE1*sin(dbLat) * sin(dbLat)));

	//ת��Ϊ�Ƕ�ֵ
	dbLon = dbLon*RAD_TO_DEG;
	dbLat = dbLat*RAD_TO_DEG;

	return true;


//#endif
}

double CGeoEllipse::GetAveRadius() const
{
	return (2*m_dbRadiusA + m_dbRadiusB)/3;
}

double CGeoEllipse::GetLat(double dbLen) const
{
	//�㷨�ο�
	//�����߻����������½�  �人��ѧѧ��
		//֣ ͮ�� ���ٷ棱

	double dbEPow2 = m_dbE1 * m_dbE1;
	double dbEpow4 = dbEPow2 * dbEPow2;
	double dbEpow6 = dbEPow2 * dbEpow4;
	double dbEpow8 = dbEpow4 * dbEpow4;

	double dbXpie2 = m_dbRadiusA*(1.0-dbEPow2)*(1 + 3/4.0*dbEPow2 + 45/64.0*dbEpow4 + 175/256.0*dbEpow6 + 11025/16384.0*dbEpow8)*(M_PI/2);
	//��γ�Ȼ�
	double dbX = dbLen/dbXpie2*(M_PI/2);

	double dbA1 = 3/8.0*dbEPow2 + 3/16.0*dbEpow4 + 213/2048.0*dbEpow6 + 255/4096.0*dbEpow8;
	double dbA2 = 21/256.0*dbEpow4 + 21/256.0*dbEpow6 + 533/8912.0*dbEpow8;
	double dbA3 = 151/6144.0*dbEpow6 + 151/4096.0*dbEpow8;
	double dbA4 = 1097/131072.0*dbEpow8;

	return (dbX + dbA1*sin(2*dbX) + dbA2*sin(4*dbX) + dbA3*sin(6*dbX) + dbA4*sin(8*dbX))*180/M_PI;
}
