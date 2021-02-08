#pragma once

#include <string>
#include "CoordCommon.h"


class COORDTRAN_API GeoEllipsoid
{
public:
	GeoEllipsoid();

	GeoEllipsoid(double dbRadiusA,double dbRadiusB);

	GeoEllipsoid(const GeoEllipsoid &ellipsoid);

	GeoEllipsoid(const std::string &name,
		const std::string &code,
		const double &dbRadiusA,
		const double &dbRadiusB,
		unsigned int epsg_code=0);

	virtual ~GeoEllipsoid(void);

	const std::string& GetName() const{return m_theName;}
	const std::string& GetCode() const{return m_theCode;}

	const double& GetA() const{return m_dbRadiusA;}
	const double& GetB() const{return m_dbRadiusB;}
	const double& GetFlattening() const{return m_theFlattening;}

	void SetA(double a){m_dbRadiusA = a;ComputeFlattening();}
	void SetB(double b){m_dbRadiusB = b;ComputeFlattening();}
	void SetAB(double a, double b){m_dbRadiusA = a; m_dbRadiusB = b; ComputeFlattening();}
	void SetEpsgCode(unsigned int code) {m_theEpsgCode = code;}


	unsigned int GetEpsgCode() const;

	double GeodeticRadius(const double& latitude) const;

	//î��Ȧ���ʰ뾶,N
	double GetPrimeRadius(double lat) const;

	//����Ȧ���ʰ뾶,M
	double GetMeridianRadius(double lat) const;

	//γ��Ȧ�İ뾶
	double GetLatCycleRadius(double lat) const;

	//γ�߻���������ָ��γ��dbLat�Ͼ��Ȳ�ΪdbDLon��γ�߻���
	double GetParallelArcLen(double dbLat,double dbDLon) const;

	//�����߻���
	double GetMeridianArcLen(double dbLat) const;

	double GetLat(double dbLen) const;

	//�������������,������γ�ߺ���������֮�����е��������
	double GetTrapeziumArea(double dbLat1,double dbLat2,double dbLon1,double dbLon2) const;


	void LatLonHeightToXYZ(double lat, double lon, double height,
		double &x, double &y, double &z) const;

	void XYZToLatLonHeight(double x, double y, double z,
		double& lat, double& lon, double& height) const;


	bool operator ==(const GeoEllipsoid& rhs) const
	{
		return ( (m_theName == rhs.m_theName)&&
			(m_theCode == rhs.m_theCode)&&
			(m_dbRadiusA    == rhs.m_dbRadiusA)&&
			(m_dbRadiusB    == rhs.m_dbRadiusB)&&
			(m_theFlattening == rhs.m_theFlattening));
	}

	bool operator!=(const GeoEllipsoid& rhs) const
	{
		return ( (m_theName != rhs.m_theName)||
			(m_theCode != rhs.m_theCode)||
			(m_dbRadiusA    != rhs.m_dbRadiusA)||               
			(m_dbRadiusB    != rhs.m_dbRadiusB)||
			(m_theFlattening != rhs.m_theFlattening));
	}

	const GeoEllipsoid& operator=(const GeoEllipsoid& rhs);

private:
	void ComputeFlattening()
	{
		m_theFlattening = (m_dbRadiusA - m_dbRadiusB)/m_dbRadiusA;
	}

	std::string m_theName;			//����
	std::string m_theCode;			//���
	mutable unsigned m_theEpsgCode;	//EPSG����
	double    m_dbRadiusA;           //������
	double    m_dbRadiusB;           //�̰���
	double    m_theFlattening;	//����
	double		m_dbE1;				//��һƫ����
	double		m_dbE2;				//�ڶ�ƫ����
};
