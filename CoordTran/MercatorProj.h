/*******************************************************************************
* 版权所有(C) pyhcx 2013
* 文件名称	: MercatorProj.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang)
* 设计日期	: 2013年10月3日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__
#define __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__

/**********************************  头文件 ************************************/

#include "GeoEllipse.h"

class COORDTRAN_API CMercatorProj
{
public:
	CMercatorProj(CGeoEllipse* pGeoEllipse,double dbCenterLon = 0,double dbStdLat = 0,
		double dbFalseE = 0,double dbFalseN = 0);
	~CMercatorProj(void);

	//墨卡托投影正算
	void LonLat2Mercator(double dbLat,double dbLon,double &x,double &y);

	//墨卡托投影反算，直接法
	void Mercator2LonLat(double x,double y,double &dbLat,double &dbLon);

	//墨卡托投影反算，迭代法
	void Mercator2LonLat2(double x,double y,double &dbLat,double &dbLon);

private:
	CGeoEllipse* m_geoEllipse;		//投影所基于的椭球体
	double m_dbCenterLon;			//原点经度
	double m_dbStdLat;				//标准纬线
	double m_dbFalseE;				//假东偏
	double m_dbFalseN;				//假北偏 
};


#endif // end of __MERCATORPROJ_H_1726EB1A_1F65_4874_BF18_7DE41FC2E6CD__
