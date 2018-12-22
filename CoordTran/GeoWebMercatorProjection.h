#pragma once

#include "GeoMapProjection.h"

class COORDTRAN_API GeoWebMercatorProjection : public GeoMapProjection
{
public:
	GeoWebMercatorProjection(const GeoEllipsoid& ellipsoid,
		const double dbOriginLat = 0,
		const double dbOriginLon = 0,
		const double dFalseEasting = 0,
		const double dFalseNorthing = 0);

	~GeoWebMercatorProjection(void);

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const ;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const ;

private:
	double m_dbCenterLat;			//起始维度
	double m_dbCenterLon;			//起始经度
	double m_dbFalseE;				//假东偏
	double m_dbFalseN;				//假北偏

	mutable double m_dMercator_a;    /* 椭球长半轴 */
	mutable double m_dMercator_f;      /* 扁率 */
	mutable double m_dMercator_e1; /* 第一偏心率    */
	mutable double m_dMercator_e2; /* 第二偏心率    */

	void SetMercatorParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing);
};
