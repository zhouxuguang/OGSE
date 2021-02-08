#include "GeoLambert2SPProjection.h"

int Sign(double dbN)
{
	if (dbN < 0)
	{
		return -1;
	}
	else
		return 1;
}


//����Mֵ
double GetValueM(double dbLat,double e1)
{
	double dbSin = sin(dbLat);
	double dbSin2 = dbSin * dbSin;
	return cos(dbLat)/sqrt(1.0-e1*e1*dbSin2);
}

//�������γ��
double GetValueT(double dbLat,double e1)
{
	double dbSin = sin(dbLat);

	return tan(M_PI_4 - dbLat*0.5)/
		pow( (1.0-e1*dbSin) / (1.0+e1*dbSin) ,e1*0.5);
}

//����Uֵ
double GetValueU(double dbLat,double e1)
{
	double dbSin = sin(dbLat);

	double dbTemp = (1.0-e1*dbSin)/(1.0+e1*dbSin);
	double dbExp = e1 * 0.5;
	double dbEquard = pow(dbTemp , dbExp);
	double dbU =  tan(M_PI_4 + dbLat * 0.5) * dbEquard;
	return dbU;
}


double GetLatCycleRadius(double dbLat,double e1,double a)
{
	double dbDenominator = sqrt(1.0-e1*e1 * sin(dbLat)*sin(dbLat));
	dbDenominator = a/dbDenominator;
	return dbDenominator * cos(dbLat);
}


GeoLambert2SPProjection::GeoLambert2SPProjection(const GeoEllipsoid& ellipsoid, 
												 const double dbStandardLat1, 
												 const double dbStandardLat2, 
												 const double dbOriginLat /*= 0*/, 
												 const double dbOriginLon /*= 0*/, 
												 const double dFalseEasting /*= 0*/, 
												 const double dFalseNorthing /*= 0*/)
												 :GeoMapProjection(ellipsoid,dbOriginLat,dbOriginLon)
{
	//����ͶӰ����
	double dMerc_Origin_Lat = dbOriginLat * DEG_TO_RAD;
	double dMerc_Origin_Long = dbOriginLon * DEG_TO_RAD;
	m_dStandardLat1 = dbStandardLat1 * DEG_TO_RAD;
	m_dStandardLat2 = dbStandardLat2 * DEG_TO_RAD;

	//�����������
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();
	SetLambertParameters(dbA,dbB,dMerc_Origin_Lat,dMerc_Origin_Long,dFalseEasting,dFalseNorthing);
}

GeoLambert2SPProjection::~GeoLambert2SPProjection(void)
{
}

void GeoLambert2SPProjection::SetLambertParameters(double a, double b, 
													double Origin_Latitude, 
													double Central_Meridian, 
													double False_Easting, 
													double False_Northing)
{
	//����ͶӰ����
	m_dLambert_False_Easting = False_Easting;
	m_dLambert_False_Northing = False_Northing;
	m_dLambert_Origin_Lat = Origin_Latitude;
	m_dLambert_Origin_Long = Central_Meridian;

	//�����������
	m_dLambert_a = a;
	m_dLambert_f = (a - b)/a;
	m_dLambert_e1 = sqrt(a*a - b*b) / a;
	m_dLambert_e2 = sqrt(a*a - b*b) / b;
}

bool GeoLambert2SPProjection::ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const
{
	assert(dbLat <= 90 && dbLat >= -90 && dbLon <= 180 && dbLon >= -180);

	//��һЩ�Ƕ�ֵת��Ϊ������
	dbLat = dbLat * DEG_TO_RAD;
	dbLon = dbLon * DEG_TO_RAD;

	//���Ȳ�
	double dbDetLon = dbLon-m_dLambert_Origin_Long;

	//����Uֵ
	double dbU1 = GetValueT(m_dStandardLat1,m_dLambert_e1);
	double dbU2 = GetValueT(m_dStandardLat2,m_dLambert_e1);
	double dbU = GetValueT(dbLat,m_dLambert_e1);
	double dbU0 = GetValueT(m_dLambert_Origin_Lat,m_dLambert_e1);

	//����Mֵ
	double dbM1 = GetValueM(m_dStandardLat1,m_dLambert_e1);
	double dbM2 = GetValueM(m_dStandardLat2,m_dLambert_e1);

	//����N
	double dbN = (log(dbM1)-log(dbM2))/(log(dbU1)-log(dbU2));

	//����Fֵ
	double dbF = dbM1/(dbN*pow(dbU1,dbN));

	double r0 = m_dLambert_a*dbF*pow(dbU0,dbN);

	//����thea
	double dbThea = dbN * dbDetLon;

	double r = m_dLambert_a*dbF*pow(dbU,dbN); 

	dbEast = r*sin(dbThea) + m_dLambert_False_Easting;
	dbNorth = r0 - r*cos(dbThea) + m_dLambert_False_Northing;

	return true;
}

bool GeoLambert2SPProjection::InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const
{
	//����Uֵ
	double dbU1 = GetValueU(m_dStandardLat1,m_dLambert_e1);
	double dbU2 = GetValueU(m_dStandardLat2,m_dLambert_e1);
	double dbU0 = GetValueU(m_dLambert_Origin_Lat,m_dLambert_e1);

	//����������׼γ�ߵĵİ뾶
	double r1 = GetLatCycleRadius(m_dStandardLat1,m_dLambert_e1,m_dLambert_a);
	double r2 = GetLatCycleRadius(m_dStandardLat2,m_dLambert_e1,m_dLambert_a);

	//���㳣����
	double dbMiu = (log(r2)-log(r1))/(log(dbU1)-log(dbU2));

	//���㳣��K
	double K = r1*pow(dbU1,dbMiu)/dbMiu;

	//����m��ֵ
	double dbM1 = GetValueM(m_dStandardLat1,m_dLambert_e1);
	double dbM2 = GetValueM(m_dStandardLat2,m_dLambert_e1);

	//����Tֵ
	double dbT1 = GetValueT(m_dStandardLat1,m_dLambert_e1);
	double dbT2 = GetValueT(m_dStandardLat2,m_dLambert_e1);
	double dbT0 = GetValueT(m_dLambert_Origin_Lat,m_dLambert_e1);

	//����N
	double dbN = (log(dbM1)-log(dbM2))/(log(dbT1)-log(dbT2));

	//����Fֵ
	double dbF = dbM1/(dbN*pow(dbT1,dbN));

	double r0 = m_dLambert_a*dbF*pow(dbT0,dbN);

	//����ͶӰԭ���ͶӰ�뾶
	double dbRou0 = K/pow(dbU0,dbMiu);

	//����r1
	double dbRou1 = Sign(dbN)*sqrt(dbEast*dbEast + (dbRou0-dbNorth)*(dbRou0-dbNorth));


	//����t1��ֵ
	double t1 = pow(dbRou1/(m_dLambert_a*dbF),1.0/dbN);

	double B0 = 1,B1 = M_PI_2;	//γ�Ƚ��е�������
	double eps = 0.000000000000001;	//����
	for (int i = 0; i < 1000; i ++)
	{
		B1 = M_PI_2 - 2* atan(t1 * pow((1.0-
			m_dLambert_e1*sin(B0))/(1.0+m_dLambert_e1*sin(B0)),m_dLambert_e1/2.0));
		if (fabs(B1-B0)>eps)
		{
			B0 = B1;
		}
		else
			break;

	}

	dbLat = B1*RAD_TO_DEG;

	//���㾭��
	double thea = atan((dbEast-m_dLambert_False_Easting)/(r0-dbNorth+m_dLambert_False_Northing));
	dbLon = (thea/dbN + m_dLambert_Origin_Long) * RAD_TO_DEG;

	return 0;
}
