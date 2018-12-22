

#include <stdio.h>
#include <math.h>
#include "GeoTranmerc.h"
#include "CoordCommon.h"

double UTMCentralMeridian (int nZone)
{
	if (nZone < 0)
	{
		return false;
	}

	double deg = -183.0 + (nZone * 6.0);
	double cmeridian = deg * DEG_TO_RAD;

	return cmeridian;
}

double GetBFLat(double sm_a,double sm_b,double dbY)
{
	double e1 = sqrt(sm_a*sm_a-sm_b*sm_b)/sm_a;
	double am = sm_a*(1.0-e1*e1);

	//首先计算各个系数的值
	double dbA1 = 1.0 + 3.0/4.0*pow(e1,2.0) + 45.0/64.0*pow(e1,4.0)+
		175.0/256.0*pow(e1,6.0) + 11025.0/16384.0*pow(e1,8.0)
		+ 43659.0/65536.0*pow(e1,10.0);

	double dbB1 = 3.0/4.0*pow(e1,2.0) + 15.0/16.0*pow(e1,4.0)+
		525.0/512.0*pow(e1,6.0) + 2205.0/2048.0*pow(e1,8.0)
		+ 72765.0/65536.0*pow(e1,10.0);

	double dbC1 = 15.0/64.0*pow(e1,4.0) + 105.0/256.0*pow(e1,6.0) + 
		2205.0/4096.0*pow(e1,8.0) + 10359.0/16384.0*pow(e1,10.0);

	double dbD1 = 35.0/512.0*pow(e1,6.0) + 315.0/2048.0*pow(e1,8.0) + 
		31185.0/13072.0*pow(e1,10.0) ;

	double dbE1 = 315.0/16384.0*pow(e1,8.0) + 3465.0/65536.0*pow(e1,10.0);

	double dbF1 = 693.0/13027.0*pow(e1,10.0);

	//double dbG1 = 1001.0/4096.0*pow(e1,12.0);

	double dbK1 = am*dbA1;
	double dbK2 = am*(-dbB1)*0.5;
	double dbK3 = am*dbC1/4.0;
	double dbK4 = am*(-dbD1)/6.0;
	double dbK5 = am*(dbE1)/8.0;
	double dbK6 = am*(-dbF1)/10.0;

	double B0 = dbY/(sm_a*(1-e1*e1)*dbA1);
	//double X0 = ArcLengthOfMeridian(B0);
	double BF = 0;
	double dbEps = fabs(BF-B0);
	do 
	{

		BF = dbY/dbK1 - dbK2 * sin(2*B0)/dbK1 - dbK3 * sin(4 * B0)/dbK1 - 
			dbK4 * sin(6 * B0)/dbK1 - dbK5 * sin(8 * B0)/dbK1 - dbK6 * sin(10 * B0)/dbK1;

		printf("fffgggff\n");

		//BF = B0 - (dbF0/dbF01) ;
		dbEps = fabs(BF-B0);
		B0 = BF;
	} while (dbEps >= 0.00000000000000001);

	return BF;
}

double ArcLengthOfMeridian(double sm_a,double sm_b,double phi)
{
	double m_dbE1 = sqrt(sm_a*sm_a - sm_b*sm_b)/sm_a;
	//首先计算各个系数的值
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

	//将角度转换为弧度
	double dbLat = phi;
    
    double dbSinTheta = sin(dbLat);
    double dbCosTheta = cos(dbLat);

	return sm_a*(1.0-pow(m_dbE1,2.0)) * 
		(dbA1*dbLat - dbCosTheta * (dbB1*dbSinTheta + dbC1*pow(dbSinTheta,3.0) +
		dbD1*pow(dbSinTheta,5.0) + dbE1*pow(dbSinTheta,7.0) + dbF1*pow(dbSinTheta,9.0) +
		dbG1*pow(dbSinTheta,11.0)));
}

static double dbTmp = 0;

void MapLatLonToXY (double sm_a,double sm_b,double phi, double lambda, double lambda0, double* xy)
{
	double N, nu2, ep2, t, t2, l;
	double l3coef, l4coef, l5coef, l6coef, l7coef, l8coef;
	double tmp;
	/* Precalculate ep2 */
	ep2 = (pow(sm_a, 2.0) - pow(sm_b, 2.0)) / pow(sm_b, 2.0);

	/* Precalculate nu2 */
	nu2 = ep2 * pow(cos(phi), 2.0);

	/* Precalculate N */
	N = pow(sm_a, 2.0) / (sm_b * sqrt(1 + nu2));

	/* Precalculate t */
	t = tan (phi);
	t2 = t * t;
	tmp = (t2 * t2 * t2) - pow (t, 6.0);

	/* 经线差 */
	l = lambda - lambda0;

	/* Precalculate coefficients for l**n in the equations below
	so a normal human being can read the expressions for easting
	and northing
	-- l**1 and l**2 have coefficients of 1.0 */
	l3coef = 1.0 - t2 + nu2;
	l4coef = 5.0 - t2 + 9 * nu2 + 4.0 * (nu2 * nu2);	 
	l5coef = 5.0 - 18.0 * t2 + (t2 * t2) + 14.0 * nu2 - 58.0 * t2 * nu2;
	l6coef = 61.0 - 58.0 * t2 + (t2 * t2) + 270.0 * nu2 - 330.0 * t2 * nu2;
	l7coef = 61.0 - 479.0 * t2 + 179.0 * (t2 * t2) - (t2 * t2 * t2);
	l8coef = 1385.0 - 3111.0 * t2 + 543.0 * (t2 * t2) - (t2 * t2 * t2); 

	/* Calculate easting (x) */
	//xy = new double[2];
	xy[0] = N * cos (phi) * l
		+ (N / 6.0 * pow (cos (phi), 3.0) * l3coef * pow (l, 3.0))
		+ (N / 120.0 * pow(cos(phi), 5.0) * l5coef * pow(l, 5.0))
		+ (N / 5040.0 * pow(cos(phi), 7.0) * l7coef * pow(l, 7.0));

	/* Calculate northing (y) */
	dbTmp = (t / 2.0 * N * pow(cos(phi), 2.0) * pow(l, 2.0))
		+ (t / 24.0 * N * pow(cos(phi), 4.0) * l4coef * pow(l, 4.0))
		+ (t / 720.0 * N * pow(cos(phi), 6.0) * l6coef * pow(l, 6.0))
		+ (t / 40320.0 * N * pow(cos(phi), 8.0) * l8coef * pow(l, 8.0));
    xy[1] = ArcLengthOfMeridian (sm_a,sm_b,phi) + dbTmp;	 
	return;
}

bool GeodeicalToMercator(double sm_a,double sm_b,
						 double dbLat, double dbLon, double dbLon0, double& easting, double& northing)
{
	if (dbLat > 90 || dbLat < -90 || dbLon > 180 || dbLon < -180)
	{
		return false;
	}

	double xy[2];

	MapLatLonToXY(sm_a,sm_b,dbLat * DEG_TO_RAD, dbLon * DEG_TO_RAD, dbLon0 , xy);

	easting = xy[0];
	northing = xy[1];

	return true;
}

bool MercatorToGeodeical(double sm_a,double sm_b,double dbX, double dbY, double dbLon0, double& dbLat, double& dbLon)
{
	//然后用求得底点纬度
	double bf = GetBFLat(sm_a,sm_b,dbY-dbTmp);

	double e2 = sqrt(sm_a*sm_a-sm_b*sm_b)/sm_a;

	double tf = tan(bf);
	double nf = sqrt(e2*e2*cos(bf)*cos(bf)/(1.0-e2*e2));
	double Vf = sqrt(1+nf*nf);
	double Nf = sm_a/sqrt( 1.0-e2*e2*sin(bf)*sin(bf) );

	dbLat = bf - ( 0.5*Vf*Vf*tf*pow(dbX/Nf,2.0) -  ( (5+3*tf*tf+nf*nf-9*pow(nf,2.0)*tf*tf)*Vf*Vf*tf*pow(dbX/Nf,4.0) ) / 24.0
		+ ( (61+90*tf*tf+45*pow(nf,4))*Vf*Vf*tf*pow(dbX/Nf,6.0) ) / 720 ) ;

	dbLat *= RAD_TO_DEG;

	dbLon = ( (dbX/Nf) - ( (1+2*tf*tf+nf*nf)/6)*pow(dbX/Nf,3) + ( (5 + 28*tf*tf + 24 * pow(tf,4) + 6*nf*nf + 8*nf*nf*tf*tf)/120 ) *pow(dbX/Nf,5) ) / cos(bf);

	//加上中央经线
	dbLon = dbLon + dbLon0;
	dbLon *= RAD_TO_DEG;
	
	return 1;
}
