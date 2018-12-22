#include "GeoEllipsoid.h"
#include "CoordCommon.h"

#include <assert.h>
#include <math.h>

GeoEllipsoid::~GeoEllipsoid(void)
{
}

GeoEllipsoid::GeoEllipsoid(double dbRadiusA,double dbRadiusB)
:m_dbRadiusA(dbRadiusA),m_dbRadiusB(dbRadiusB)
{
	//计算第一和第二偏心率
	m_dbE1 = sqrt(m_dbRadiusA*m_dbRadiusA - m_dbRadiusB*m_dbRadiusB) / m_dbRadiusA;
	m_dbE2 = sqrt(m_dbRadiusA*m_dbRadiusA - m_dbRadiusB*m_dbRadiusB) / m_dbRadiusB;
}

GeoEllipsoid::GeoEllipsoid(const GeoEllipsoid &ellipsoid)
{
	m_dbRadiusA = ellipsoid.m_dbRadiusA;
	m_dbRadiusB = ellipsoid.m_dbRadiusB;
	m_dbE1 = ellipsoid.m_dbE1;
	m_dbE2 = ellipsoid.m_dbE2;
	m_theFlattening = ellipsoid.m_theFlattening;
	m_theName = ellipsoid.m_theName;
	m_theCode = ellipsoid.m_theCode;
	m_theEpsgCode = ellipsoid.m_theEpsgCode;
}

GeoEllipsoid::GeoEllipsoid()
{

}

unsigned int GeoEllipsoid::GetEpsgCode() const
{
	return m_theEpsgCode;
}

double GeoEllipsoid::GeodeticRadius(const double& latitude) const
{
	return 0;
}

const GeoEllipsoid& GeoEllipsoid::operator=(const GeoEllipsoid& rhs)
{
	m_dbRadiusA = rhs.m_dbRadiusA;
	m_dbRadiusB = rhs.m_dbRadiusB;
	m_dbE1 = rhs.m_dbE1;
	m_dbE2 = rhs.m_dbE2;
	m_theFlattening = rhs.m_theFlattening;
	m_theName = rhs.m_theName;
	m_theCode = rhs.m_theCode;
	m_theEpsgCode = rhs.m_theEpsgCode;

	return *this;
}

void GeoEllipsoid::LatLonHeightToXYZ(double lat, double lon, double height, 
									 double &x, double &y, double &z) const
{
	//参数合法性检查
	assert(lat >= -90 && lat <= 90);
	assert(lon >= -180 && lon <= 180);

	//计算卯酉圈曲率半径
	double dbN = GetPrimeRadius(lat);

	double B = lat * DEG_TO_RAD; //转换为弧度制
	double L = lon * DEG_TO_RAD; 
	double H = height;

	double dbCosB = cos(B);
	double dbSinB = sin(B);
	double dbCosL = cos(L);
	double dbSinL = sin(L);

	//计算空间直角坐标
	x = (dbN + H)*dbCosB*dbCosL;
	y = (dbN + H)*dbCosB*dbSinL;
	z = (dbN*(1.0-m_dbE1*m_dbE1)+ H)*dbSinB;
}

void GeoEllipsoid::XYZToLatLonHeight(double X, double Y, double Z, 
									 double& lat, double& lon, double& height) const
{
	//求解经度
	lon = atan2(Y,X);

	//求解纬度
	double r = X*X + Y*Y;
	double R = Z*Z + r;

	double dbSqrtr = sqrt(r);
	double dbSqrtR = sqrt(R);

	//求解一些中间变量

	double dbU = ( m_dbRadiusB *Z * (1 + m_dbE2*m_dbE2*m_dbRadiusB/dbSqrtR) ) / (m_dbRadiusA*dbSqrtr);
	dbU = atan(dbU);

	double dbSinU = sin(dbU);
	double dbSinUPow3 = dbSinU * dbSinU * dbSinU;
	double dbCosU = cos(dbU);
	double dbCosUPow3 = dbCosU * dbCosU * dbCosU;
	
	double dbTemp = (Z + m_dbE2*m_dbE2*m_dbRadiusB* dbSinUPow3 ) / 
				(dbSqrtr - m_dbE1*m_dbE1*m_dbRadiusA*dbCosUPow3);

	lat = atan(dbTemp);

	double dbSinLat = sin(lat);
	double dbCosLat = cos(lat);
	double dbSinLatPow2 = dbSinLat * dbSinLat;

	//求解高程
	height = dbSqrtr*dbCosLat + Z*dbSinLat - 
			m_dbRadiusA*(sqrt(1.0-m_dbE1*m_dbE1*dbSinLatPow2));

	//转换为角度值
	lon = lon*RAD_TO_DEG;
	lat = lat*RAD_TO_DEG;

}

double GeoEllipsoid::GetPrimeRadius(double lat) const
{
	lat = lat * DEG_TO_RAD;

	double dbDenominator = sqrt(1.0-m_dbE1*m_dbE1 * sin(lat)*sin(lat));
	return m_dbRadiusA/dbDenominator;
}

double GeoEllipsoid::GetMeridianRadius(double lat) const
{
	//先将纬度化作弧度制
	lat = lat *  acos(-1.0)/180.0;
	double dbNumerator = m_dbRadiusA*(1.0-m_dbE1*m_dbE1); 
	double dbDenominator = pow(1.0 - m_dbE1*m_dbE1 * sin(lat)*sin(lat),1.5);
	return dbNumerator/dbDenominator;
}

double GeoEllipsoid::GetLatCycleRadius(double lat) const
{
	assert(lat >= -90 && lat <= 90);
	double dbN = GetPrimeRadius(lat);
	lat = lat * acos(-1.0)/180.0;
	return dbN * cos(lat);
}

double GeoEllipsoid::GetParallelArcLen(double dbLat,double dbDLon) const
{
	//检查参数
	assert(dbLat >= -90 && dbLat <= 90
		&& dbDLon <= 180 && dbDLon >= 0);

	double dbN = GetPrimeRadius(dbLat);
	dbLat = dbLat * acos(-1.0)/180.0;
	dbDLon = dbDLon * acos(-1.0)/180.0;

	return dbN*cos(dbLat)*dbDLon;
}

double GeoEllipsoid::GetMeridianArcLen(double dbLat) const
{
	//检测参数是否合法
	assert(dbLat >= -90 && dbLat <= 90);

//#ifdef __linux__
//
//	//首先计算各个系数的值
//	double dbA1 = 1.0 + 3.0/4.0*pow(m_dbE1,2.0) + 45.0/64.0*pow(m_dbE1,4.0)+
//		175.0/256.0*pow(m_dbE1,6.0) + 11025.0/16384.0*pow(m_dbE1,8.0)
//		+ 43659.0/65536.0*pow(m_dbE1,10.0) + 693693/1048576.0*pow(m_dbE1,12.0);
//
//	double dbB1 = 3.0/4.0*pow(m_dbE1,2.0) + 45.0/64.0*pow(m_dbE1,4.0)+
//		175.0/256.0*pow(m_dbE1,6.0) + 11025.0/16384.0*pow(m_dbE1,8.0)
//		+ 43659.0/65536.0*pow(m_dbE1,10.0) + 693693/1048576.0*pow(m_dbE1,12.0);
//
//	double dbC1 = 15.0/32.0*pow(m_dbE1,4.0) + 175.0/384.0*pow(m_dbE1,6.0) + 
//		3675.0/8192.0*pow(m_dbE1,8.0) + 14553.0/32768.0*pow(m_dbE1,10.0) + 
//		231231.0/524288.0*pow(m_dbE1,12.0);
//
//	double dbD1 = 35.0/96.0*pow(m_dbE1,6.0) + 735.0/2048.0*pow(m_dbE1,8.0) + 
//		14553.0/40960.0*pow(m_dbE1,10.0) + 231231.0/655360.0*pow(m_dbE1,12.0);
//
//	double dbE1 = 315.0/1024.0*pow(m_dbE1,8.0) + 6237.0/20480.0*pow(m_dbE1,10.0) + 
//		99099.0/327680.0*pow(m_dbE1,12.0);
//
//	double dbF1 = 693.0/2560.0*pow(m_dbE1,10.0) + 11011.0/40960.0*pow(m_dbE1,12.0);
//
//	double dbG1 = 1001.0/4096.0*pow(m_dbE1,12.0);
//
//	//将角度转换为弧度
//	dbLat = (dbLat * acos(-1.0)/180.0);
//
//	return m_dbRadiusA*(1.0-pow(m_dbE1,2.0)) * 
//		(dbA1*dbLat - cos(dbLat) * (dbB1*sin(dbLat) + dbC1*pow(sin(dbLat),3.0) + 
//		dbD1*pow(sin(dbLat),5.0) + dbE1*pow(sin(dbLat),7.0) + dbF1*pow(sin(dbLat),9.0) + 
//		dbG1*pow(sin(dbLat),11.0)));
//
//#else

	//算法参考：<<地理信息系统算法基础>>
	double dbEPow2 = m_dbE1*m_dbE1;
	double dbEPow4 = dbEPow2*dbEPow2;
	double dbEPow6 = dbEPow2*dbEPow4;
	double dbEPow8 = dbEPow6*dbEPow2;
	double dbEPow10 = dbEPow8*dbEPow2;
	double dbEPow12 = dbEPow10*dbEPow2;

	//首先计算各个系数的值
	double dbA1 = 1.0 + 3.0/4.0*dbEPow2 + 45.0/64.0*dbEPow4+
		175.0/256.0*dbEPow6 + 11025.0/16384.0*dbEPow8
		+ 43659.0/65536.0*dbEPow10 + 693693.0/1048576.0*dbEPow12;

	double dbB1 = 3.0/8.0*dbEPow2 + 15.0/32.0*dbEPow4 + 
		525.0/1024.0*dbEPow6 + 2205.0/4096.0*dbEPow8
		+ 72765.0/131072.0*dbEPow10 + 297297.0/524288.0*dbEPow12;

	double dbC1 = 15.0/256.0*dbEPow4 + 105.0/1024.0*dbEPow6 + 
		2205.0/16384.0*dbEPow8 + 10395.0/65536.0*dbEPow10 + 1486485.0/8388608.0*dbEPow12;

	double dbD1 = 35.0/3072.0*dbEPow6 + 105.0/4096.0*dbEPow8 + 10395.0/262144.0*dbEPow10 + 55055.0/1048576.0*dbEPow12;

	double dbE1 = 315.0/131072.0*dbEPow8 + 3465.0/524288.0*dbEPow10 + 99099.0/8388608.0*dbEPow12;

	double dbF1 =  639.0/1310720.0*dbEPow10 + 9009.0/5242880.0*dbEPow12;

	double dbG1 = 1001.0/8388608.0*dbEPow12;

	//将角度转换为弧度
	dbLat *= DEG_TO_RAD;

	return m_dbRadiusA*(1.0-dbEPow2) * (dbA1*dbLat - dbB1*sin(2*dbLat) + dbC1*sin(4*dbLat) - 
		dbD1*sin(6*dbLat) + dbE1*sin(8*dbLat) - dbF1*sin(10*dbLat) + dbG1*sin(12*dbLat));

//#endif
}

double GeoEllipsoid::GetLat(double dbLen) const
{
	//算法参考
	//子午线弧长反问题新解  武汉大学学报
	//郑 彤１ 边少锋１

	double dbEPow2 = m_dbE1 * m_dbE1;
	double dbEpow4 = dbEPow2 * dbEPow2;
	double dbEpow6 = dbEPow2 * dbEpow4;
	double dbEpow8 = dbEpow4 * dbEpow4;
	double dbEPow10 = dbEpow4 * dbEpow6;
	double dbEPow12 = dbEPow10 * dbEPow2;

	double dbXpie2 = m_dbRadiusA*(1.0-dbEPow2)*(1 + 3/4.0*dbEPow2 + 45/64.0*dbEpow4 + 175/256.0*dbEpow6 + 
		11025/16384.0*dbEpow8 /*+ 43659.0/65536.0*dbEPow10 + 693693.0/1048576.0*dbEPow12*/)*(M_PI_2);
	//先纬度化
	double dbX = dbLen/dbXpie2*(M_PI_2);

	double dbA1 = 3/8.0*dbEPow2 + 3/16.0*dbEpow4 + 213/2048.0*dbEpow6 + 255/4096.0*dbEpow8;
	double dbA2 = 21/256.0*dbEpow4 + 21/256.0*dbEpow6 + 533/8912.0*dbEpow8;
	double dbA3 = 151/6144.0*dbEpow6 + 151/4096.0*dbEpow8;
	double dbA4 = 1097/131072.0*dbEpow8;
	double dbA5 = 3014/2000014*dbEPow10;

	return (dbX + dbA1*sin(2*dbX) + dbA2*sin(4*dbX) + dbA3*sin(6*dbX) + dbA4*sin(8*dbX) /*+ dbA5*sin(10*dbX)*/)*180/M_PI;
}

double GeoEllipsoid::GetTrapeziumArea(double dbLat1,double dbLat2,double dbLon1,double dbLon2) const
{
	//检查参数是否合法
	assert(dbLat1 >= -90 && dbLat1 <= 90
		&& dbLon1 <= 180 && dbLon1 >= -180);
	assert(dbLat2 >= -90 && dbLat2 <= 90
		&& dbLon2 <= 180 && dbLon2 >= -180);

	//计算相关的系数
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

	//计算纬度差，中间纬度等
	dbLat1 = dbLat1 * acos(-1.0)/180.0;
	dbLat2 = dbLat2 * acos(-1.0)/180.0;
	double dbDetLat = fabs(dbLat2-dbLat1);
	double dbMidLat = (dbLat1+dbLat2)/2.0;

	//计算结果
	return dbK * (dbA*sin(dbDetLat/2.0)*cos(dbMidLat) - dbB*sin(3*dbDetLat/2.0)*cos(3*dbMidLat)
		+ dbC*sin(5*dbDetLat/2.0)*cos(5*dbMidLat) - dbD*sin(7*dbDetLat/2.0)*cos(7*dbMidLat) + 
		dbE*sin(9*dbDetLat/2.0)*cos(9*dbMidLat)) ;
}
