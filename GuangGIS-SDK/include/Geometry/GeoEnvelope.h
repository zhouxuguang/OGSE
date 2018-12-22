/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoEnvelope.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang@126.com)
* 设计日期	: 2012年3月6日
* 内容摘要	: 地理空间矩形类，可以表达几何体的轴对齐最小外界矩形
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__
#define __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__

#include "GeoDefine.h"

class GeoCoordinate;

class GEOMETRY_API GeoEnvelope
{
public:
	//默认构造函数
	GeoEnvelope();

	//带参数的构造函数
	GeoEnvelope(double minX,double maxX,double minY,double maxY);

	//拷贝构造函数
	GeoEnvelope(const GeoEnvelope& envelope);

	//重载赋值运算符
	GeoEnvelope &operator=(const GeoEnvelope& other);

	//用两个坐标点初始化
	GeoEnvelope(GeoCoordinate *coord1,GeoCoordinate *coord2);

	~GeoEnvelope(void);

	//初始化矩形
	void Init(double x1,double x2,double y1,double y2);


	// 判断矩形是否为空
	bool IsNull(void) const;

	// 获取最小外包矩形的宽度
	double GetWidth(void) const;

	// 获取最小外界矩形的高度
	double GetHeight(void) const;

	// 获得矩形的中心点坐标
	bool Center(GeoCoordinate &coord) const;

	bool Center(double &x,double &y) const;

	//测试是否包含另一个MBR
	bool Contains(const GeoEnvelope &env) const;

	//判断一个点是否在该矩形中
	bool Contains(const GeoCoordinate &pt) const;

	//是否包含这个点
	bool Contains(double x, double y) const;

	//判断一个点是否在矩形内
	DEPRECATE_API bool IsPointInRect(double x,double y) const;

	//计算两个矩形相交的部分
	bool Intersection(const GeoEnvelope& env,GeoEnvelope &envResult) const;

	//计算两个矩形是否相交
	bool Intersects(const GeoEnvelope *pOther) const;

	bool Intersects(const GeoEnvelope &env) const;

	//矩形增长
	void ExpandToInclude(double x, double y);

	void ExpandToInclude(const GeoCoordinate &pt);

	void ExpandToInclude(const GeoEnvelope &other);

	void ExpandToInclude(const GeoEnvelope *other);

	//计算到另一个MBR的距离
	double DistanceTo(GeoEnvelope &env) const;

	//计算面积
	double GetArea() const;

	//计算周长
	double Perimeter() const;

	//变换，平移
	void Translate(double transX, double transY);

	//静态函数

	//判断p1,p2构成的矩形和q1,q2构成的矩形是否相交
	static bool Intersects(const GeoCoordinate &p1, const GeoCoordinate &p2, const GeoCoordinate &q1, const GeoCoordinate &q2);

public:
	double minX;	//最小外包矩形的最小x值
	double maxX;	//最小外包矩形的最大x值
	double minY;	//最小外包矩形的最小y值
	double maxY;	//最小外包矩形的最大y值
};

#endif // end of __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__
