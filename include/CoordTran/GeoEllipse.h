/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2013
* 文件名称	: GeoEllipse.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zxg)
* 设计日期	: 2013年9月22日
* 内容摘要	: 地球椭球体计算，主要有一些参数的计算和转换关系
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__
#define __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__

/**********************************  头文件 ************************************/


#include <math.h>
#include <assert.h>
#include "CoordCommon.h"


class COORDTRAN_API CGeoEllipse
{
public:
	explicit CGeoEllipse(double dbRadiusA,double dbRadiusB);
	~CGeoEllipse(void);

	double GetRadiusA() const
	{
		return m_dbRadiusA;
	}

	double GetRadiusB() const
	{
		return m_dbRadiusB;
	}

	double GetE1() const
	{
		return m_dbE1;
	}

	double GetE2() const
	{
		return m_dbE2;
	}

	//子午圈曲率半径,M
	double GetMeridianRadius(double lat) const;

	//卯酉圈曲率半径,N
	double GetPrimeRadius(double lat) const;

	//纬线圈的半径
	double GetLatCycleRadius(double lat) const;

	//平均曲率半径
	double GetAveRadius() const;

	//纬线弧长，即在指定纬度dbLat上经度差为dbDLon的纬线弧长
	double GetParallelArcLen(double dbLat,double dbDLon) const;

	//子午线弧长
	double GetMeridianArcLen(double dbLat) const;

	double GetLat(double dbLen) const;

	//椭球体梯形面积,即两条纬线和两条经线之间所夹的梯形面积
	double GetTrapeziumArea(double dbLat1,double dbLat2,double dbLon1,double dbLon2) const;

	//计算U值，dbLat以弧度为单位，等量纬度
	double GetValueU(double dbLat);

	//大地坐标转换为空间直角坐标
	bool BLH_XYZ(double dbLon,double dbLat,double dbHei,double& x,double &y,double &z);

	//空间直角坐标转换为大地坐标
	bool XYZ_BLH(double X,double Y,double Z, double &dbLon,double &dbLat,double &dbHei);

private:
	double m_dbRadiusA;			//长半轴
	double m_dbRadiusB;			//短半轴
	double m_dbE1;				//第一偏心率
	double m_dbE2;				//第二偏心率
};


#endif // end of __GEOELLIPSE_H_05958735_A7BF_409C_A444_CF9F202FCBE9__
