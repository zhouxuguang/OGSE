#ifndef GEOLAMBERT_PROJECTION_2_FKKJKLLLDDD_INCLUDE_H
#define GEOLAMBERT_PROJECTION_2_FKKJKLLLDDD_INCLUDE_H

#include "GeoMapProjection.h"

class COORDTRAN_API GeoLambert2SPProjection :public GeoMapProjection
{
public:
	GeoLambert2SPProjection(const GeoEllipsoid& ellipsoid,
		const double dbStandardLat1,
		const double dbStandardLat2,
		const double dbOriginLat = 0,
		const double dbOriginLon = 0,
		const double dFalseEasting = 0,
		const double dFalseNorthing = 0);

	~GeoLambert2SPProjection(void);

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const ;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const ;

private:
	mutable double m_dLambert_a;    /* 椭球长半轴 */
	mutable double m_dLambert_f;      /* 扁率 */
	mutable double m_dLambert_e1; /* 第一偏心率    */
	mutable double m_dLambert_e2; /* 第二偏心率    */

	/* 兰伯特投影参数 */
	mutable double m_dLambert_Origin_Lat;      /* 起始点纬度的弧度数     */
	mutable double m_dLambert_Origin_Long;     /* 起始点经度的弧度数    */
	mutable double m_dLambert_False_Northing;  /* 假东偏多少米          */
	mutable double m_dLambert_False_Easting;   /* 假北偏多少米         */
	mutable double m_dLambert_Scale_Factor;    /* 比例系数                     */
	mutable double m_dStandardLat1;   /* 第一标准纬线         */
	mutable double m_dStandardLat2;    /* 第二标准纬线                     */

	void SetLambertParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing);
};

#endif
