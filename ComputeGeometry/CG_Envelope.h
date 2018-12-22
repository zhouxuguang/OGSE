/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoEnvelope.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang@126.com)
* 设计日期	: 2011年10月6日
* 内容摘要	: 最小包围盒
* 修改记录	:
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __G_ENVELOPE_H_118FB5E8_C3C6_42F6_BE56_D37A882316F8__
#define __G_ENVELOPE_H_118FB5E8_C3C6_42F6_BE56_D37A882316F8__

/**********************************  头文件 ************************************/

#include "CG_Common.h"


class COMPUTEGEOMETRY_API CG_Envelope
{
public:
	//默认构造函数
	CG_Envelope();

	//带参数的构造函数
	CG_Envelope(double minX,double maxX,double minY,double maxY);

	//拷贝构造函数
	CG_Envelope(const CG_Envelope& envelope);

	//用两个坐标点初始化
	CG_Envelope(const SPOINT &coord1,const SPOINT & coord2);

	~CG_Envelope(void);

	//判断两个最小外包矩形是否相交(计算出在内还在外)
	int InterSects(const CG_Envelope & otherEvp);

	// 判断矩形是否为空
	bool IsNull(void) const;

	// 获取最小外包矩形的宽度
	double GetWidth(void) const;

	// 获取最小外界矩形的高度
	double GetHeight(void) const;

	// 获得矩形的中心点坐标
	SPOINT &Center() const;

	bool Center(double *pX,double *pY) const;

	//测试是否包含另一个MBR
	bool Contains(const CG_Envelope &env) const;

	//判断一个点是否在该矩形中
	bool Contains(const SPOINT &pt) const;

	//是否包含这个点
	bool Contains(double x, double y) const;

	//判断一个点是否在矩形内
	bool IsPointInRect(double x,double y) const;

	//计算两个矩形相交的部分
	CG_Envelope& Intersection(const CG_Envelope& env);

	//计算到另一个MBR的距离
	double DistanceTo(CG_Envelope &env);

	//计算面积
	double Area() const;

	//计算周长
	double Perimeter() const;

	//静态函数

	//判断p1,p2构成的矩形和q1,q2构成的矩形是否相交
	static bool Intersects(const SPOINT &p1, const SPOINT &p2, 
		const SPOINT &q1, const SPOINT &q2);

	static bool Intersects(const CG_Envelope &env1, const CG_Envelope &env2);

public:
	double minX;	//最小外包矩形的最小x值
	double maxX;	//最小外包矩形的最大x值
	double minY;	//最小外包矩形的最小y值
	double maxY;	//最小外包矩形的最大y值
};

#endif // end of __GEOENVELOPE_H_E9CBC39A_81BD_4AC7_BD29_EB1D0D58CF03__

