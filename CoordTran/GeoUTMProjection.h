#ifndef GEOUTMPROJECTION_INCLUDE_H
#define GEOUTMPROJECTION_INCLUDE_H


#include "GeoMapProjection.h"

class COORDTRAN_API GeoUTMProjection : public GeoMapProjection
{
public:
	GeoUTMProjection(const GeoEllipsoid& ellipsoid,
		const double dbOriginLat,
		const double dbOriginLon,
		int nZone,
		char cHemisphere,
		const double dFalseEasting = 500000.0,
		const double dFalseNorthing = 0);

	~GeoUTMProjection(void);

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const ;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const ;

private:
	double m_dbTranMerc_a;  /* 长半轴 */
	double m_dbTranMerc_b;  /* 长半轴 */
	double m_dbTranMerc_f;  /* 扁率 */

	double m_dbTranMerc_e;	/*第一偏心率*/

	/* 通用横轴墨卡托投影的参数 */
	double m_dbTranMerc_Origin_Lat;     /* 原点纬度，弧度 */
	double m_dbTranMerc_Origin_Long;    /* 原点经度，弧度 */
	double m_dbTranMerc_False_Northing; /* 假北偏多少米 */
	double m_dbTranMerc_False_Easting;  /* 假东偏多少米 */
	double m_dbTranMerc_Scale_Factor;   /* 缩放系数 */

	int m_nZone;	/*投影的带号*/
	char  m_cHemisphere; /*南北半球标示*/

	void SetMercatorParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing,
		int nZone,
		char cHemisphere);
};

#endif
