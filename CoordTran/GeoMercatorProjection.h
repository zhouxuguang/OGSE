#ifndef GEOMERCATOR_PROJECTION_INCLUDE_H
#define GEOMERCATOR_PROJECTION_INCLUDE_H


#include "GeoMapProjection.h"

class GeoEllipsoid;

class COORDTRAN_API GeoMercatorProjection : public GeoMapProjection
{
public:
	GeoMercatorProjection(const GeoEllipsoid& ellipsoid ,
		double dbOriginLat =0,double dbOriginLon = 0);

	GeoMercatorProjection(const GeoEllipsoid& ellipsoid,
		const double dbOriginLat,
		const double dbOriginLon,
		const double dFalseEasting,
		const double dFalseNorthing);

	~GeoMercatorProjection(void);

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const ;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const ;

private:

	mutable double m_dMerc_a;    /* 椭球长半轴 */
	mutable double m_dMerc_f;      /* 扁率 */
	mutable double m_dMerc_e1; /* 第一偏心率    */
	mutable double m_dMerc_e2; /* 第二偏心率    */

	/* 墨卡托投影参数 */
	mutable double m_dMerc_Origin_Lat;      /* 起始点纬度的弧度数     */
	mutable double m_dMerc_Origin_Long;     /* 起始点经度的弧度数    */
	mutable double m_dMerc_False_Northing;  /* 假东偏多少米          */
	mutable double m_dMerc_False_Easting;   /* 假北偏多少米         */
	mutable double m_dMerc_Scale_Factor;    /* 比例系数                     */

	void SetMercatorParameters(double a,      
		double b,
		double Origin_Latitude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing);

	long Convert_Geodetic_To_Mercator (double Latitude,
		double Longitude,
		double *Easting,
		double *Northing) const;

	long Convert_Mercator_To_Geodetic(double Easting,
		double Northing,
		double *Latitude,
		double *Longitude) const;
};

#endif
