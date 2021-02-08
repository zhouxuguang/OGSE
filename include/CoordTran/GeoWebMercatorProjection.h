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
	double m_dbCenterLat;			//��ʼά��
	double m_dbCenterLon;			//��ʼ����
	double m_dbFalseE;				//�ٶ�ƫ
	double m_dbFalseN;				//�ٱ�ƫ

	mutable double m_dMercator_a;    /* ���򳤰��� */
	mutable double m_dMercator_f;      /* ���� */
	mutable double m_dMercator_e1; /* ��һƫ����    */
	mutable double m_dMercator_e2; /* �ڶ�ƫ����    */

	void SetMercatorParameters(double a,      
		double b,
		double Origin_Latiude,
		double Central_Meridian,
		double False_Easting,
		double False_Northing);
};
