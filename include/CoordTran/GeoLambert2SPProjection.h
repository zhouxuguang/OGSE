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
	mutable double m_dLambert_a;    /* ���򳤰��� */
	mutable double m_dLambert_f;      /* ���� */
	mutable double m_dLambert_e1; /* ��һƫ����    */
	mutable double m_dLambert_e2; /* �ڶ�ƫ����    */

	/* ������ͶӰ���� */
	mutable double m_dLambert_Origin_Lat;      /* ��ʼ��γ�ȵĻ�����     */
	mutable double m_dLambert_Origin_Long;     /* ��ʼ�㾭�ȵĻ�����    */
	mutable double m_dLambert_False_Northing;  /* �ٶ�ƫ������          */
	mutable double m_dLambert_False_Easting;   /* �ٱ�ƫ������         */
	mutable double m_dLambert_Scale_Factor;    /* ����ϵ��                     */
	mutable double m_dStandardLat1;   /* ��һ��׼γ��         */
	mutable double m_dStandardLat2;    /* �ڶ���׼γ��                     */

	void SetLambertParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing);
};

#endif
