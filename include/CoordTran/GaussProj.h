/*******************************************************************************
* 版权所有(C) pyhcx 2013
* 文件名称	: GaussProj.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2013年10月10日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__
#define __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__

/**********************************  头文件 ************************************/


class COORDTRAN_API CGaussProj
{
public:
	CGaussProj(CGeoEllipse* pGeoEllipse,double dbCenterLat,double dbCenterLon,
		double dbFalseN,double dbFalseE,int nType);
	~CGaussProj(void);

	//地理坐标转换为高斯平面直角坐标
	bool LonLat2Gauss(int zoneWide,double dbLat,double dbLon,double *x,double *y);

	//高斯平面直角坐标转换地理坐标
	bool Gauss2LonLat(double x,double y,double *dbLat,double *dbLon);

private:
	CGeoEllipse* m_pGeoEllipse;
	double m_dbCenterLat;			//中央纬线
	double m_dbCenterLon;			//中央经线
	double m_dbFalseN;				//北向偏移
	double m_dbFalseE;				//东向偏移
	int m_nType;			//3 or 6
};

#endif // end of __GAUSSPROJ_H_21BDBAEF_009C_4736_9233_91C3AD980EC5__
