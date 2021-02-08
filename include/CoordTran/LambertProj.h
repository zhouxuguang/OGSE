/*******************************************************************************
* 版权所有(C) pyhcx 2013
* 文件名称	: LambertProj.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang@)
* 设计日期	: 2013年10月1日
* 内容摘要	: 主要用于兰伯特投影计算
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__
#define __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__

/**********************************  头文件 ************************************/

#include "CoordCommon.h"

class CGeoEllipse;


class COORDTRAN_API CLambertProj
{
public:
	CLambertProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
		double dbStdLat1,double dbStdLat2,
		double dbFalseE = 0,double dbFalseN = 0);
	~CLambertProj(void);

	//兰伯特投影正算
	void LatLon2Lambert(double dbLat,double dbLon,double &x,double &y);

	//兰伯特投影反算
	void Lambert2LatLon(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//投影所基于的椭球体
	double m_dbCenterLat;			//起始纬度
	double m_dbCenterLon;			//起始经度
	double m_dbStdLat1;				//第一标准纬线
	double m_dbStdLat2;				//第二标准纬线
	double m_dbFalseE;				//假东偏
	double m_dbFalseN;				//假北偏 
};

#endif // end of __LAMBERTPROJ_H_59CAE94A_E937_42AD_AA27_794E467715BB__

//计算U值，dbLat以弧度为单位
extern "C" double GetValueU(CGeoEllipse* pGeoEllipse,double dbLat);

//符号函数
extern "C" int Sign(double dbN);

//计算M的值,弧度为单位
extern "C" double GetValueM(CGeoEllipse* pGeoEllipse,double dbLat);

//计算T值
extern "C" double GetValueT(CGeoEllipse* pGeoEllipse,double dbLat);
