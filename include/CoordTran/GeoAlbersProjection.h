#ifndef GEOALBERSPROJECTION_INCLUDE_H
#define GEOALBERSPROJECTION_INCLUDE_H


#include "GeoMapProjection.h"

class COORDTRAN_API GeoAlbersProjection : public GeoMapProjection
{
public:
	GeoAlbersProjection(const GeoEllipsoid& ellipsoid,
		double Origin_Lat,
		double Origin_Long,
		double stdParallel1,
		double stdParallel2,
		double falseEasting,
		double falseNorthing);

	~GeoAlbersProjection(void);

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const ;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const ;

private:
	double m_dAlbers_Origin_Lat;            /* ԭ��γ��  ����   */
	double m_dAlbers_Origin_Long;               /* ԭ�㾭�� ����    */
	double m_dAlbers_Std_Parallel_1;		/*��һ��׼γ��*/
	double m_dAlbers_Std_Parallel_2;		/*�ڶ���׼γ��*/
	double m_dAlbers_False_Easting;			/*�ٶ�ƫ����*/
	double m_dAlbers_False_Northing;		/*�ٱ�ƫ����*/

	//�������
	double m_dbAlbers_a;  /* ������ */
	double m_dbAlbers_b;  /* ������ */
	double m_dbAlbers_f;  /* ���� */

	double m_dbAlbers_e1;	/*��һƫ����*/
	double m_dbAlbers_e2;	/*�ڶ�ƫ����*/

	double C;                     /* constant c   */
	double rho0;                 /* height above ellipsoid		*/
	double n;                    /* ratio between meridians		*/
	double Albers_a_OVER_n;      /* Albers_a / n */
	double one_MINUS_es2;        /* 1 - es2 */
	double two_es;               /* 2 * es */

	void SetAlbersParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double dbStd_Parallel_1,
		double dbStd_Parallel_2,
		double False_Easting,
		double False_Northing);

	//�������ת��Ϊ�Ƕ���˹����
	long Convert_Geodetic_To_Albers (double Latitude,
		double Longitude,
		double *Easting,
		double *Northing) const;

	//�Ƕ���˹����ת��Ϊ�������
	long Convert_Albers_To_Geodetic(double Easting,
		double Northing,
		double *Latitude,
		double *Longitude) const;
};

#endif
