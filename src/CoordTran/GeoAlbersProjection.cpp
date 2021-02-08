#include "GeoAlbersProjection.h"


#define ES_SIN(sinlat)          (m_dbAlbers_e1 * sinlat)
#define ONE_MINUS_SQR(x)        (1.0 - x * x)
#define ALBERS_M(clat,oneminussqressin)   (clat / sqrt(oneminussqressin))
#define ALBERS_Q(slat,oneminussqressin,essin) (one_MINUS_es2)*(slat / (oneminussqressin)-    \
	(1 / (two_es)) *log((1 - essin) / (1 + essin)))


GeoAlbersProjection::GeoAlbersProjection(const GeoEllipsoid& ellipsoid, double Origin_Lat, double Origin_Long, 
										 double stdParallel1, double stdParallel2, double falseEasting, double falseNorthing)
										 :GeoMapProjection(ellipsoid)
{
	double dbA = ellipsoid.GetA();
	double dbB = ellipsoid.GetB();
	Origin_Lat = Origin_Lat*DEG_TO_RAD;
	Origin_Long = Origin_Long*DEG_TO_RAD;

	stdParallel1 = stdParallel1*DEG_TO_RAD;
	stdParallel2 = stdParallel2*DEG_TO_RAD;
	SetAlbersParameters(dbA,dbB,Origin_Lat,Origin_Long,stdParallel1,stdParallel2,falseEasting,falseNorthing);
}

GeoAlbersProjection::~GeoAlbersProjection(void)
{
}

void GeoAlbersProjection::SetAlbersParameters(double a, double b, double Origin_Latiude, double Central_Meridian,
											  double dbStd_Parallel_1,
											  double dbStd_Parallel_2,
											  double False_Easting, double False_Northing)
{
	m_dbAlbers_a = a;
	m_dbAlbers_b = b;
	m_dbAlbers_e1 = sqrt(a*a - b*b)/a;
	m_dbAlbers_e2 = sqrt(a*a - b*b)/b;

	m_dAlbers_Origin_Lat = Origin_Latiude;
	if (Central_Meridian > M_PI)
		Central_Meridian -= TWO_PI;
	m_dAlbers_Origin_Long = Central_Meridian;
	m_dAlbers_Std_Parallel_1 = dbStd_Parallel_1;
	m_dAlbers_Std_Parallel_2 = dbStd_Parallel_2;
	m_dAlbers_False_Easting = False_Easting;
	m_dAlbers_False_Northing = False_Northing;

	//计算相关的投影参数
	one_MINUS_es2 = 1 - m_dbAlbers_e1*m_dbAlbers_e1;
	two_es = 2 * m_dbAlbers_e1;

	double sin_lat = sin(m_dAlbers_Origin_Lat);
	double es_sin = ES_SIN(sin_lat);
	double one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
	double q0 = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);

	double sin_lat_1 = sin(m_dAlbers_Std_Parallel_1);
	double cos_lat = cos(m_dAlbers_Std_Parallel_1);
	es_sin = ES_SIN(sin_lat_1);
	one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
	double m1 = ALBERS_M(cos_lat, one_MINUS_SQRes_sin);
	double q1 = ALBERS_Q(sin_lat_1, one_MINUS_SQRes_sin, es_sin);

	double SQRm1 = m1 * m1;
	if (fabs(m_dAlbers_Std_Parallel_1 - m_dAlbers_Std_Parallel_2) > 1.0e-10)
	{
		sin_lat = sin(m_dAlbers_Std_Parallel_2);
		cos_lat = cos(m_dAlbers_Std_Parallel_2);
		es_sin = ES_SIN(sin_lat);
		one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
		double m2 = ALBERS_M(cos_lat, one_MINUS_SQRes_sin);
		double q2 = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);
		n = (SQRm1 - m2 * m2) / (q2 - q1);
	}
	else
		n = sin_lat_1;

	C = SQRm1 + n * q1;
	Albers_a_OVER_n = m_dbAlbers_a / n;
	double nq0 = n * q0;
	if (C < nq0)
		rho0 = 0;
	else
		rho0 = Albers_a_OVER_n * sqrt(C - nq0);
}

bool GeoAlbersProjection::ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const
{
	Convert_Geodetic_To_Albers(dbLat*DEG_TO_RAD,dbLon*DEG_TO_RAD,&dbEast,&dbNorth);
	return 1;
}

bool GeoAlbersProjection::InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const
{
	Convert_Albers_To_Geodetic(dbEast,dbNorth,&dbLat,&dbLon);
	dbLat *= RAD_TO_DEG;
	dbLon *= RAD_TO_DEG;
	return 1;
}

long GeoAlbersProjection::Convert_Geodetic_To_Albers(double Latitude, double Longitude, double *Easting, double *Northing) const
{
	double dlam;                      /* 经度 - 中央经度 */
	double sin_lat, cos_lat;
	double es_sin, one_MINUS_SQRes_sin;
	double q;
	double rho;
	double theta;
	double nq;
	long Error_Code = 0;

	if ((Latitude < -M_PI_2) || (Latitude > M_PI_2))
	{  /* Latitude out of range */
		Error_Code |= 1;
	}
	if ((Longitude < -M_PI) || (Longitude > TWO_PI))
	{  /* Longitude out of range */
		Error_Code|= 1;
	}

	if (!Error_Code)
	{ /* no errors */

		dlam = Longitude - m_dAlbers_Origin_Long;
		if (dlam > M_PI)
		{
			dlam -= TWO_PI;
		}
		if (dlam < -M_PI)
		{
			dlam += TWO_PI;
		}
		sin_lat = sin(Latitude);
		cos_lat = cos(Latitude);
		es_sin = ES_SIN(sin_lat);
		one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
		q = ALBERS_Q(sin_lat, one_MINUS_SQRes_sin, es_sin);
		nq = n * q;
		if (C < nq)
			rho = 0;
		else
			rho = Albers_a_OVER_n * sqrt(C - nq);


		theta = n * dlam;
		*Easting = rho * sin(theta) + m_dAlbers_False_Easting;
		*Northing = rho0 - rho * cos(theta) + m_dAlbers_False_Northing;
	}
	return (Error_Code);
}

long GeoAlbersProjection::Convert_Albers_To_Geodetic(double Easting, double Northing, double *Latitude, double *Longitude) const
{
	double dy, dx;
	double rho0_MINUS_dy;
	double q, qconst, q_OVER_2;
	double rho, rho_n;
	double PHI, Delta_PHI = 1.0;
	double sin_phi;
	double es_sin, one_MINUS_SQRes_sin;
	double theta = 0.0;
	double tolerance = 4.85e-10;        /* approximately 1/1000th of an arc second or 1/10th meter */
	long Error_Code = 0; 

	//if ((Easting < (Albers_False_Easting - Albers_Delta_Easting)) 
	//    || (Easting > Albers_False_Easting + Albers_Delta_Easting))
	//{ /* Easting out of range  */
	//  Error_Code |= ALBERS_EASTING_ERROR;
	//}
	//if ((Northing < (Albers_False_Northing - Albers_Delta_Northing)) 
	//    || (Northing > Albers_False_Northing + Albers_Delta_Northing))
	//{ /* Northing out of range */
	//  Error_Code |= ALBERS_NORTHING_ERROR;
	//}

	if (!Error_Code)
	{
		dy = Northing - m_dAlbers_False_Northing;
		dx = Easting - m_dAlbers_False_Easting;
		rho0_MINUS_dy = rho0 - dy;
		rho = sqrt(dx * dx + rho0_MINUS_dy * rho0_MINUS_dy);

		if (n < 0)
		{
			rho *= -1.0;
			dy *= -1.0;
			dx *= -1.0;
			rho0_MINUS_dy *= -1.0;
		}

		if (rho != 0.0)
			theta = atan2(dx, rho0_MINUS_dy);
		rho_n = rho * n;
		q = (C - (rho_n * rho_n) / (m_dbAlbers_a * m_dbAlbers_a)) / n;
		qconst = 1 - ((one_MINUS_es2) / (two_es)) * log((1.0 - m_dbAlbers_e1) / (1.0 + m_dbAlbers_e1));
		if (fabs(fabs(qconst) - fabs(q)) > 1.0e-6)
		{
			q_OVER_2 = q / 2.0;
			if (q_OVER_2 > 1.0)
				*Latitude = M_PI_2;
			else if (q_OVER_2 < -1.0)
				*Latitude = -M_PI_2;
			else
			{
				PHI = asin(q_OVER_2);
				if (m_dbAlbers_e1 < 1.0e-10)
					*Latitude = PHI;
				else
				{
					while (fabs(Delta_PHI) > tolerance)
					{
						sin_phi = sin(PHI);
						es_sin = ES_SIN(sin_phi);
						one_MINUS_SQRes_sin = ONE_MINUS_SQR(es_sin);
						Delta_PHI = (one_MINUS_SQRes_sin * one_MINUS_SQRes_sin) / (2.0 * cos(PHI)) *
							(q / (one_MINUS_es2) - sin_phi / one_MINUS_SQRes_sin +
							(log((1.0 - es_sin) / (1.0 + es_sin)) / (two_es)));
						PHI += Delta_PHI;
					}
					*Latitude = PHI;
				}

				if (*Latitude > M_PI_2)  /* force distorted values to 90, -90 degrees */
					*Latitude = M_PI_2;
				else if (*Latitude < -M_PI_2)
					*Latitude = -M_PI_2;

			}
		}
		else
		{
			if (q >= 0.0)
				*Latitude = M_PI_2;
			else
				*Latitude = -M_PI_2;
		}
		*Longitude = m_dAlbers_Origin_Long + theta / n;

		if (*Longitude > M_PI)
			*Longitude -= TWO_PI;
		if (*Longitude < -M_PI)
			*Longitude += TWO_PI;

		if (*Longitude > M_PI) /* force distorted values to 180, -180 degrees */
			*Longitude = M_PI;
		else if (*Longitude < -M_PI)
			*Longitude = -M_PI;

	}
	return (Error_Code);
}
