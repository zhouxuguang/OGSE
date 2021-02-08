#pragma once

#include "CoordCommon.h"

class CGeoEllipse;

class COORDTRAN_API HammerProj
{
public:
	HammerProj(CGeoEllipse* pGeoEllipse,double dbCenterLon);
	~HammerProj(void);

	//Hammer投影正算
	void LatLon2Hammer(double dbLat,double dbLon,double &x,double &y);

	//Hammer投影反算
	void Hammer2LatLon(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//投影所基于的椭球体
	double m_dbCenterLon;			//中央子午线经度
};
