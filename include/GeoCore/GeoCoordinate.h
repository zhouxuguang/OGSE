/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoCoordinate.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年6月7日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__
#define __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__

/**********************************  头文件 ************************************/

#include "GeoDefine.h"

class GEOMETRY_API GeoCoordinate
{
public:
	//默认构造函数
	GeoCoordinate(void);
	//带参数的构造函数
	GeoCoordinate(double x,double y);
	//拷贝构造函数
	GeoCoordinate(const GeoCoordinate& coord);
	//析构函数
	~GeoCoordinate(void);

	//计算到另一个点得距离
	double Distance(GeoCoordinate &coord) const;

	//设置X和Y坐标
	void SetX(double x);
	void SetY(double y);

public:
	double x;	//横坐标
	double y;	//纵坐标
	//double z;	//z坐标
};

#endif // end of __GEOCOORDINATE_H_FFF7BBFB_E6D0_439F_A6CE_561AD1EE0958__
