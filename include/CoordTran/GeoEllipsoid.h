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

	//卯酉圈曲率半径,N
	double GetPrimeRadius(double lat) const;

	//子午圈曲率半径,M
	double GetMeridianRadius(double lat) const;

	//纬线圈的半径
	double GetLatCycleRadius(double lat) const;

	//纬线弧长，即在指定纬度dbLat上经度差为dbDLon的纬线弧长
	double GetParallelArcLen(double dbLat,double dbDLon) const;

	//子午线弧长
	double GetMeridianArcLen(double dbLat) const;

	double GetLat(double dbLen) const;

	//椭球体梯形面积,即两条纬线和两条经线之间所夹的梯形面积
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

	std::string m_theName;			//名字
	std::string m_theCode;			//编号
	mutable unsigned m_theEpsgCode;	//EPSG编码
	double    m_dbRadiusA;           //长半轴
	double    m_dbRadiusB;           //短半轴
	double    m_theFlattening;	//扁率
	double		m_dbE1;				//第一偏心率
	double		m_dbE2;				//第二偏心率
};
