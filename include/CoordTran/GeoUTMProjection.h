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
	double m_dbTranMerc_a;  /* ������ */
	double m_dbTranMerc_b;  /* ������ */
	double m_dbTranMerc_f;  /* ���� */

	double m_dbTranMerc_e;	/*��һƫ����*/

	/* ͨ�ú���ī����ͶӰ�Ĳ��� */
	double m_dbTranMerc_Origin_Lat;     /* ԭ��γ�ȣ����� */
	double m_dbTranMerc_Origin_Long;    /* ԭ�㾭�ȣ����� */
	double m_dbTranMerc_False_Northing; /* �ٱ�ƫ������ */
	double m_dbTranMerc_False_Easting;  /* �ٶ�ƫ������ */
	double m_dbTranMerc_Scale_Factor;   /* ����ϵ�� */

	int m_nZone;	/*ͶӰ�Ĵ���*/
	char  m_cHemisphere; /*�ϱ������ʾ*/

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
