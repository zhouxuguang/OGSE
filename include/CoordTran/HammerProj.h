#pragma once

#include "CoordCommon.h"

class CGeoEllipse;

class COORDTRAN_API HammerProj
{
public:
	HammerProj(CGeoEllipse* pGeoEllipse,double dbCenterLon);
	~HammerProj(void);

	//HammerͶӰ����
	void LatLon2Hammer(double dbLat,double dbLon,double &x,double &y);

	//HammerͶӰ����
	void Hammer2LatLon(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//ͶӰ�����ڵ�������
	double m_dbCenterLon;			//���������߾���
};
