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
	double m_dAlbers_Origin_Lat;            /* 原点纬度  弧度   */
	double m_dAlbers_Origin_Long;               /* 原点经度 弧度    */
	double m_dAlbers_Std_Parallel_1;		/*第一标准纬线*/
	double m_dAlbers_Std_Parallel_2;		/*第二标准纬线*/
	double m_dAlbers_False_Easting;			/*假东偏移量*/
	double m_dAlbers_False_Northing;		/*假北偏移量*/

	//椭球参数
	double m_dbAlbers_a;  /* 长半轴 */
	double m_dbAlbers_b;  /* 长半轴 */
	double m_dbAlbers_f;  /* 扁率 */

	double m_dbAlbers_e1;	/*第一偏心率*/
	double m_dbAlbers_e2;	/*第二偏心率*/

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

	//大地坐标转换为亚尔波斯坐标
	long Convert_Geodetic_To_Albers (double Latitude,
		double Longitude,
		double *Easting,
		double *Northing) const;

	//亚尔波斯坐标转换为大地坐标
	long Convert_Albers_To_Geodetic(double Easting,
		double Northing,
		double *Latitude,
		double *Longitude) const;
};

#endif
